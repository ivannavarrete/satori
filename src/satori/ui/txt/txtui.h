
#ifndef TXTUI_H
#define TXTUI_H


#include <QPluginLoader>
#include <string>
#include "lib/command/command.h"


/**
 * This class provides an abstract text user interface. The interface is
 * implemented by different UI plugins to provide new capabilities such as
 * support for a new architecture or communication domain.
 */
class TxtUi {
public:
	/**
	 * There is no need for a virtual destructor in an interface class, but
	 * some compilers generate warnings without it.
	 */
	virtual ~TxtUi() {}

	/**
	 * The execute method that takes a @a command and executes it.
	 *
	 * @param command		command to execute
	 */
	virtual void Exec(const Command &command) = 0;

	/**
	 * This method takes a Command object and initializes it to a command
	 * from the UI's command table. It searches the command table for a command
	 * named @a command_name and copies it's attributes to @a command. If the
	 * command was not found the @a command object is unchanged.
	 *
	 * @param command		command object to initialize
	 * @param command_name	name of the command to search for
	 * 
	 * @return				True if command was found and initialized, false
	 * 						otherwise.
	 */
	virtual bool Find(Command &command,const std::string &command_name) const=0;
};


Q_DECLARE_INTERFACE(TxtUi, "com.angulum.satori.TxtUi/1.0");


#endif
