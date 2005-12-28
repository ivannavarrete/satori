
#ifndef COMMAND_H
#define COMMAND_H


#include <string>
#include <list>
#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include "lib/tree.h"
#include "argument.h"
#include "wordargument.h"
#include "stringargument.h"
#include "numberargument.h"
//#include "commandtable.h"


/**
 *
 * Any std::logic_error exceptions in this object are because of the object
 * being corrupt or the *CommandTable and *TxtUI objects being out of sync,
 * meaning that *TxtUI accesses arguments in a way not supported by the command.
 * Neither of these causes should occur in a correct application. Since this
 * class is used in both plugins and main application it uses static_cast<> and
 * it's own rtti on arguments (Argument::Type()) , instead of dynamic_cast<>.
 * This is the cause of most std::logic_error exceptions.
 *
 * Any std::invalid_argument exceptions are the cause of errors in parsing the
 * command line of the user. This will occur frequently in normal use and
 * should be dealt with at runtime.
 *
 *
 * @todo Document.
 */
class Command {
private:
	typedef tree<boost::shared_ptr<Argument> > ArgumentTree;
	typedef std::list<boost::shared_ptr<Argument> > ArgumentList;

public:
	Command(const std::string &n, const int type=CommandType::NoOp,
							const std::string &short_description="",
							const std::string &long_description="");
	Command &operator=(const Command &command);

	void AddArguments(ArgumentList &argument_list);
	void ParseArguments(std::string command_line);

	const std::string &Name() const;
	const int Type() const { return type; }
	const std::string &ShortDescription() const { return short_description; }
	const std::string &LongDescription() const { return long_description; }

	unsigned int ArgumentCount() const { return argument_path.size(); }
	bool IsValid(const unsigned int i) const;
	bool IsValid(const unsigned int i, const int type) const;
	uint32_t GetNumber(const unsigned int i) const;
	const std::string &GetWord(const unsigned int i) const;
	const std::string &GetString(const unsigned int i) const;

private:
	void AddArguments(ArgumentList &sub_list, ArgumentTree &sub_tree);
	void ParseArguments(std::string &arguments, ArgumentTree &sub_tree);

	int type;
	std::string short_description;
	std::string long_description;

	boost::shared_ptr<ArgumentTree> argument_tree;
	std::vector<boost::shared_ptr<Argument> > argument_path;
};


#endif
