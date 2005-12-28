
#ifndef COMMANDTABLE_H
#define COMMANDTABLE_H


#include <vector>
#include <boost/shared_ptr.hpp>


class Command;


namespace CommandType {
	const int NoOp = 0;
}


/**
 * This is the base class for all *CommandTable classes. It holds a set of
 * commands and provides methods for searching and iterating through the
 * command table.
 */
class CommandTable {
public:
	/// const iterator for iterating through the commands in command table
	typedef std::vector<boost::shared_ptr<Command> >::const_iterator
											const_command_iterator;

	inline const_command_iterator begin() const { return command_table.begin();}
	inline const_command_iterator end() const { return command_table.end(); }

	bool Find(Command &command, const std::string &command_name) const;

protected:
	std::vector<boost::shared_ptr<Command> > command_table;
};


#endif
