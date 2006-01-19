
#include "command.h"


/**
 * Create a command. The command will be initialized with a name, type and
 * command descriptions. It can then be further populated with arguments with
 * the AddArguments() method.
 *
 * @param name					name of the command
 * @param type					command type as specified in CommandTable
 * @param short_description		short command description
 * @param long_description		long command description
 *
 * @throw std::invalid_argument	This exception is thrown if @a name is not a
 * 								valid word as defined in the WordArgument class.
 *
 * @todo Consider removing the default value for type, and thus try to only
 *		 create new commands in the *CommandTable classes.
 */
Command::Command(const std::string &name, const int type, const std::string &short_description, const std::string &long_description) {
	this->type = type;
	this->short_description = short_description;
	this->long_description = long_description;

	// create new argument that holds the command name
	boost::shared_ptr<Argument> command_argument(new WordArgument);
	command_argument->SetType(name);

	// create the argument tree and put command argument at the top node
	argument_tree = boost::shared_ptr<ArgumentTree>
				(new ArgumentTree(command_argument));
}


/**
 * outstream operator.
 *
 * insert this in mock class:
 *		friend ostream &operator<<(ostream &out_stream, const Command &command);
 *
 * @todo Move this to a mock class for testing this class. This method does
 *		 not belong here.
 */
/*
ostream &operator<<(ostream &out_stream, const Command &command) {
	out_stream << "command: "
		<< static_cast<WordArgument &>(*command.argument_tree->value()).Value()
		<< "\n  " <<  *(command.argument_tree);

	return out_stream;
}
*/


/**
 * Assignment operator. Copy the command type and argument tree.
 *
 * @param command		command object to copy from
 *
 * @return				reference to this command object
 */
Command &Command::operator=(const Command &command) {
	type = command.type;
	short_description = command.short_description;
	long_description = command.long_description;

	// @todo Figure out which option is the best; copy the tree pointer or tree
	*argument_tree = *(command.argument_tree);
	//argument_tree = command.argument_tree;

	// @todo Consider clearing the argument_path, or creating new copies of
	//		 Argument objects to insert into argument path. Right now the
	//		 two different commands will have the _same_ Arguments in their
	//		 argument paths, which is almost certainly a bad idea.
	//argument_path = command.argument_path;
	argument_path.clear();

	return *this;
}


/**
 * Add the arguments in the @a argument_list to the command. The list should
 * represent the arguments in a command invocation. This method should be
 * called for every different type of command invocation.
 *
 * For example, a command of type:
 *		command <String> | (<Number> <String> (<Array> | <Number>))
 *
 * would have to be initialized with three calls to this method (list denoted
 * by []):
 *		AddArguments([String]);
 *		AddArguments([Number, String, Array]);
 *		AddArguments([Number, String, Number]);
 *
 * @param argument_list			list of arguments
 * 
 * @todo Consider making a copy of the argument_list instead of destroying it,
 *		 i.e. passing it by value. Edit: Perhaps not a good idea since it
 *		 contains shared_ptr:s.
 */
void Command::AddArguments(ArgumentList &argument_list) {
	AddArguments(argument_list, *argument_tree);
}


/**
 * This method adds the arguments in the @a sub_list to the @a sub_tree.
 *
 * @param sub_list				list of arguments
 * @param sub_tree				tree to add arguments to
 */
void Command::AddArguments(ArgumentList &sub_list, ArgumentTree &sub_tree) {
	if (!sub_list.empty()) {
		// try to find a child element of the sub_tree that matches the type
		// of the first element in the list
		for (ArgumentTree::child_iterator child_tree_i = sub_tree.child_begin();
			 child_tree_i != sub_tree.child_end(); ++child_tree_i) {

			// if the argument type at the start of the list is the same as
			// one of the children of the tree, remove that argument from the
			// list and add the rest of the list to that subtree
			if (sub_list.front()->Type() == (*child_tree_i).value()->Type()) {
				sub_list.erase(sub_list.begin());
				AddArguments(sub_list, *child_tree_i);
				return;
			}
		}

		// if there was no child in the subtree matching the first list element
		// then create a new child (sub tree)
		//
		// Note: add_child() will make a copy of the argument given to it and
		// insert it into the new node, but since the arguments we're dealing
		// with in the list and trees are of type shared_ptr<>, only the
		// pointer itself will be copied, not the object it points to. Since
		// we remove the pointer from the list in the next statement, only
		// the tree will hold the object and thus no weirdness can occur.
		// If we wanted to keep the object in the list, it would probably
		// be necessary to make a copy of the object manually and then insert
		// it into the new tree wrapped in a new shared_ptr<>
		ArgumentTree &new_sub_tree = sub_tree.add_child(sub_list.front());
		sub_list.erase(sub_list.begin());
		AddArguments(sub_list, new_sub_tree);
	}
}


/**
 * Parse the @a command_line string and initialize the command object. After
 * successfully parsing the command line the Command object is transformed from
 * a general command description to a specific command instance with it's
 * variable arguments initialized. The caller can then access the parsed
 * arguments through the GetNumber(), GetWord() and GetString() methods.
 * The command_line is passed by value since it will be destroyed when parsed.
 *
 * @param command_line			string holding the command line
 *
 * @throw std::invalid_argument	Thrown if the parsed @a command_line does not
 * 								match a path in the argument tree for this
 * 								command.
 * @throw std::logic_error		Thrown if the command object is corrupt; i.e.
 * 								the command name is not a proper WordArgument.
 *
 * @todo Maybe a different exception would be more appropriate.
 */
void Command::ParseArguments(std::string command_line) {
	boost::shared_ptr<Argument> name_argument = argument_tree->value();

	// compare the command name with the first argument on command_line
	if (Name() != name_argument->Extract(command_line))
		throw std::invalid_argument("wrong command name");

	// add command name argument to argument path
	argument_path.clear();
	argument_path.push_back(name_argument);

	// parse the rest of the command line
	ParseArguments(command_line, *argument_tree);
}


/**
 * Parse the @a arguments string using the @a sub_tree. The parsed arguments
 * must correspond to a path in the @a sub_tree. Upon successful completion
 * the @a sub_tree  will have a 'list' structure; each node having only one
 * child which is initialized to the value of the corresponding parsed argument.
 * The @a arguments string is destroyed.
 *
 * @param arguments				string holding the arguments to parse
 * @param sub_tree				argument tree to search for argument path
 *
 * @throw std::invalid_argument	Thrown if the parsed @a arguments do not match a
 *	 							path in the argument tree @a sub_tree.
 *
 * @todo Maybe a different exception would be more appropriate.
 */
void Command::ParseArguments(std::string &arguments, ArgumentTree &sub_tree) {
	// search through all children to see which one can parse the next argument
	// from the arguments string
	for (ArgumentTree::child_iterator child_tree_i = sub_tree.child_begin();
		 child_tree_i != sub_tree.child_end(); ++child_tree_i) {

		// use current Argument object to extract the next argument from the
		// arguments string
		boost::shared_ptr<Argument> argument = child_tree_i->value();
		std::string next_argument = argument->Extract(arguments);
		
		// if the argument object at the child node can parse the extracted
		// argument string then do so, make a new copy of the argument to put
		// into path, and continue parsing
		if (argument->IsType(next_argument)) {
			// an argument can change the type of the command
			if (argument->Modifier() != CommandType::NoOp)
				type = argument->Modifier();

			// this if block makes sure the underlying argument objects are
			// copied.. if this is removed, and it should be considered,
			// then there must not be more than one command of the same type
			// alive at the same time, because their parsings would intermix
			if (argument->Type() == Argument::Word) {
				WordArgument &arg = static_cast<WordArgument &>(*argument);
				argument = boost::shared_ptr<Argument>(new WordArgument(arg));
			} else if (argument->Type() == Argument::String) {
				StringArgument &arg = static_cast<StringArgument &>(*argument);
				argument = boost::shared_ptr<Argument>(new StringArgument(arg));
			} else if (argument->Type() == Argument::Number) {
				NumberArgument &arg = static_cast<NumberArgument &>(*argument);
				argument = boost::shared_ptr<Argument>(new NumberArgument(arg));
			} else {
				// @todo Can't happen, but throw exception or something.
			}

			argument->SetType(next_argument);
			argument_path.push_back(argument);
			ParseArguments(arguments, *child_tree_i);
			return;
		}
	}

	// all remaining arguments must be optional and have no children
	for (ArgumentTree::child_iterator child_tree_i = sub_tree.child_begin();
		 child_tree_i != sub_tree.child_end(); ++child_tree_i) {
		if (child_tree_i->value()->Opt() == Argument::Mandatory) {
			if (arguments.find_first_not_of(" \t\n") != std::string::npos)
				throw std::invalid_argument("bad argument type");
			else
				throw std::invalid_argument("too few arguments");
		} else {
			if (arguments.find_first_not_of(" \t\n") != std::string::npos)
				throw std::invalid_argument("bad argument type");
		}
	}

	// excessive arguments on the command line
	if (arguments.find_first_not_of(" \t\n") != std::string::npos)
		throw std::invalid_argument("too many arguments");
}


/**
 * Get the name of the command.
 *
 * @return						Reference to command name string.
 *
 * @throw std::logic_error		Thrown if the command object is corrupt; i.e.
 * 								the command name is not a proper WordArgument.
 */
const std::string &Command::Name() const {
	Argument &name_argument = *(argument_tree->value());
	if (name_argument.Type() != Argument::Word)
		throw std::logic_error("corrupt Command: command name not a word");

	return (static_cast<WordArgument &>(name_argument)).Value();
}


/**
 * Check whether the @a i:th argument is accessible and valid. This method
 * should be called after a call to ParseArguments().
 *
 * @param i						the index of the argument to check
 *
 * @return						True if argument is accessible and valid, false
 * 								otherwise.
 */
bool Command::IsValid(const unsigned int i) const {
	try {
		argument_path.at(i);
	} catch (std::out_of_range &e) {
		return false;
	}

	return true;
}


/**
 *
 */
bool Command::IsValid(const unsigned int i, const int type) const {
	try {
		if (argument_path.at(i)->Type() == type)
			return true;
		return false;
	} catch (std::out_of_range &e) {
		return false;
	}
}


/**
 * Get the number from the number argument at index @a i. This method should be
 * called after a call to ParseArguments().
 *
 * @param i						argument index
 *
 * @return						Extracted number.
 *
 * @throw std::logic_error		Thrown if there was an error getting the number
 * 								value. This happens if the @a i index was out
 * 								of range, or the argument at index @a i is not
 * 								a number argument.
 */
uint32_t Command::GetNumber(const unsigned int i) const {
	try {
		Argument &argument = *(argument_path.at(i));
		if (argument.Type() == Argument::Number)
			return (static_cast<NumberArgument &>(argument)).Value();
		
		throw std::logic_error("bad argument: not a number");
	} catch (std::out_of_range &e) {
		throw std::logic_error("bad argument: index out of range");
	}
}


/**
 * Get the word from the word argument at index @a i. This method should be
 * called after a call to ParseArguments().
 *
 * @param i						argument index
 *
 * @return						Reference to word string.
 * 
 * @throw std::logic_error		Thrown if there was an error getting the word
 * 								value from the argument tree. This happens if
 * 								the @a i index was out of range, or the argument
 * 								at index @a i is not a word argument.
 */
const std::string &Command::GetWord(const unsigned int i) const {
	try {
		Argument &argument = *(argument_path.at(i));
		if (argument.Type() == Argument::Word)
			return (static_cast<WordArgument &>(argument)).Value();

		throw std::logic_error("bad argument: not a word");
	} catch (std::out_of_range &e) {
		throw std::logic_error("bad argument: index out of range");
	}
	
}


/**
 * Get the string from the string argument at index @a i. This method should be
 * called after a call to ParseArguments().
 *
 * @param i						argument index
 *
 * @return						Reference to argument string.
 * 
 * @throw std::logic_error		Thrown if there was an error getting the string
 * 								value from the argument tree. This happens if
 * 								the @a i index was out of range, or the argument
 * 								at index @a i is not a string argument.
 */
const std::string &Command::GetString(const unsigned int i) const {
	try {
		Argument &argument = *(argument_path.at(i));
		if (argument.Type() == Argument::String)
			return (static_cast<StringArgument &>(argument)).Value();

		throw std::logic_error("bad argument: not a string");
	} catch (std::out_of_range &e) {
		throw std::logic_error("bad argument: index out of range");
	}
}


/**
 * Get the char from the character argument at index @a i. This method should be
 * called after a call to ParseArguments().
 *
 * @param i						argument index
 *
 * @return						Reference to char.
 * 
 * @throw std::logic_error		Thrown if there was an error getting the char
 * 								value from the argument tree. This happens if
 * 								the @a i index was out of range, or the argument
 * 								at index @a i is not a character argument.
 */
const char &Command::GetCharacter(const unsigned int i) const {
	try {
		Argument &argument = *(argument_path.at(i));
		if (argument.Type() == Argument::Character)
			return (static_cast<CharacterArgument &>(argument)).Value();

		throw std::logic_error("bad argument: not a character");
	} catch (std::out_of_range &e) {
		throw std::logic_error("bad_argument: index out of rande");
	}
}
