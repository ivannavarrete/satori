
#include <QCoreApplication>
#include <QDir>
#include <iostream>
#include "basetxtui.h"
#include "color.h"


/**
 * This is the command loop that reads and executes user commands.
 */
void BaseTxtUi::Idle() {
	Message("Satori version 0.2.0");
	Message("Welcome");
	Message();

	while (true) {
		// get user input
		std::cout << Color::Prompt << ": " << Color::Default;

		std::string command_line;
		getline(std::cin, command_line);
		boost::tokenizer<> tokens(command_line);
		if (tokens.begin() == tokens.end())
			continue;

		// execute commands
		try {
			Command command("null_command");

			// execute architecture specific commands
			bool exec_arch_ui = arch_ui != NULL &&
								arch_ui->Find(command, *tokens.begin());
			if (exec_arch_ui) {
				command.ParseArguments(command_line);
				arch_ui->Exec(command);
			}
			
			// execute communication specific commands
			bool exec_comm_ui = comm_ui != NULL &&
								comm_ui->Find(command, *tokens.begin());
			if (exec_comm_ui) {
				command.ParseArguments(command_line);
				comm_ui->Exec(command);
			}

			// execute architecture independent commands
			bool exec_base_ui = Find(command, *tokens.begin());
			if (exec_base_ui) {
				command.ParseArguments(command_line);
				Exec(command);

				// the only command that's handled both here and in Exec()
				if (command.Type() == BaseCommandTable::Quit)
					return;
			}
			
			// display error if no subsystem executed the command
			if (!exec_base_ui && !exec_comm_ui && !exec_arch_ui)
				SyntaxError(std::string("command not found"));


		// handle ParseArguments() and Command constructor failure
		} catch (std::invalid_argument e) {
			SyntaxError(e.what());
		} catch (std::bad_cast e) {
			Error(std::string("bad_cast exception: ") + e.what());
			exit(1);
		} catch (std::logic_error e) {
			Error(std::string("logic_error exception: ") + e.what());
			exit(1);
		} catch (...) {
			Error("unhandled exception");
			exit(1);
		}
	}
}


/**
 *
 */
bool BaseTxtUi::Find(Command &command, const std::string &command_name) const{
	return command_table.Find(command, command_name);
}


/**
 * Execute the command specified by the @a command object.
 *
 * @param command		command and its arguments
 */
void BaseTxtUi::Exec(const Command &command) {
	switch (command.Type()) {
	case (BaseCommandTable::Quit):
		CommandQuit();
		break;
	case (BaseCommandTable::ShowModule):
		CommandShowModule();
		break;
	case (BaseCommandTable::LoadModule):
		CommandLoadModule(command);
		break;
	case (BaseCommandTable::ClearScreen):
		CommandClearScreen();
		break;
	case (BaseCommandTable::Help):
		CommandHelp(command);
		break;
	default:
		CommandUnknown();
		break;
	}
}


/**
 * Cleanup method that's called when program terminates.
 */
void BaseTxtUi::CommandQuit() const {
	QCoreApplication::exit(0);		// stop event processing
}


/**
 *
 */
void BaseTxtUi::CommandShowModule() const {
	QDir module_dir =QDir(QCoreApplication::applicationDirPath()+"/../modules");
	if (!module_dir.exists()) {
		Error("can't find module directory");
		return;
	}

	/// @bug It could happen that arch dir does not exist and then the next
	///		 if statement will search for comm dir in the wrong place.
	///		 Also, we should probably output error if module dirs do not
	///		 exist.

	// display all working architecture modules
	if (module_dir.cd(tr("arch"))) {
		Message("--[ Architecture modules ]--");

		foreach (QString module_name, module_dir.entryList(QDir::Dirs)) {
			QPluginLoader loader(module_dir.absoluteFilePath(
						module_name+"/lib"+module_name+"txt.so"));
			QObject *module = loader.instance();
			if (module) {
				TxtUi *ui_if = qobject_cast<TxtUi *>(module);
				CommUser *commuser_if = qobject_cast<CommUser *>(module);
				if (ui_if && commuser_if)
					Message(module_name.toStdString());

				if (!loader.unload())
					Error("can't unload module");
			}
		}
	}

	// display all working communication modules
	if (module_dir.cd(tr("../comm"))) {
		Message("--[ Communication modules ]--");
		foreach (QString module_name, module_dir.entryList(QDir::Dirs)) {
			QPluginLoader loader(module_dir.absoluteFilePath(
						module_name+"/lib"+module_name+".so"));
			QObject *module = loader.instance();
			if (module) {
				TxtUi *ui_if = qobject_cast<TxtUi *>(module);
				CommProvider *commprov_if = qobject_cast<CommProvider*>(module);
				if (ui_if && commprov_if)
					Message(module_name.toStdString());

				if (!loader.unload())
					Error("can't unload module");
			}
		}
	}
}


/**
 * Load a communication or architecture module.
 *
 * @param command				Command object holding module name
 *
 * @Todo: Find out exceptions thrown.
 *
 * @Todo: Find out what happens if there are a comm and arch module with the
 * 		  same name
 */
void BaseTxtUi::CommandLoadModule(const Command &command) {
	const QString module_name(command.GetWord(1).c_str());
	bool loaded = false;

	// try loading an architecture module
	QDir modules_dir(QCoreApplication::applicationDirPath()+"/../modules/arch");
	if (!modules_dir.exists()) {
		Error("can't find module directory");
		return;
	}

	foreach (QString module_dir, modules_dir.entryList(QDir::Dirs)) {
		if (module_dir == "." || module_dir == "..")
			continue;

		if (module_dir == module_name) {
			QPluginLoader loader(modules_dir.absoluteFilePath(module_dir +
											"/lib" + module_name + "txt.so"));
			QObject *module = loader.instance();
			if (module) {
				// load the arch module if it implements both these interfaces
				arch_ui = qobject_cast<TxtUi *>(module);
				comm_user = qobject_cast<CommUser *>(module);
				if (arch_ui && comm_user) {
					loaded = true;
					Message("architecture module loaded: " +
									module_name.toStdString());

					// if a comm module is already loaded then we get it's Comm
					// object and pass it to the newly loaded arch module
					if (comm_provider)
						comm_user->SetComm(comm_provider->GetComm());
				} else {
					arch_ui = NULL;
					comm_user = NULL;
				}
			}
		}
	}


	// try loading a communication module
	modules_dir.setPath(QCoreApplication::applicationDirPath()+
											"/../modules/comm");
	if (!modules_dir.exists()) {
		Error("can't find module directory");
		return;
	}

	foreach (QString module_dir, modules_dir.entryList(QDir::Dirs)) {
		if (module_dir == "." || module_dir == "..")
			continue;

		if (module_dir == module_name) {
			QPluginLoader loader(modules_dir.absoluteFilePath(module_dir +
									"/lib" + module_name + ".so"));
			QObject *module = loader.instance();
			if (module) {
				// load the comm module if it implements both these interfaces
				comm_ui = qobject_cast<TxtUi *>(module);
				comm_provider = qobject_cast<CommProvider *>(module);
				
				if (comm_ui && comm_provider) {
					loaded = true;
					Message("communication module loaded: " +
									module_name.toStdString());

					// if an arch module is already loaded then we set it's Comm
					// object with the one from the newly loaded comm module
					if (comm_user)
						comm_user->SetComm(comm_provider->GetComm());
				} else {
					comm_ui = NULL;
					comm_provider = NULL;
				}
			}
		}
	}

	if (!loaded)
		Error("failed loading module [" + command.GetWord(1) +
										"]: module not found");
}


/**
 * Clear screen.
 * 
 * @Todo If possible, find out the screen geometry at runtime (number of rows),
 * for more precise screen clearing.
 */
void BaseTxtUi::CommandClearScreen() const {
	std::cout << Color::Prompt
			  << "]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n"
				 "]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n"
				 "]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n";
}


/**
 * Display help.
 *
 * @param command				Command object holding optional command name
 *
 * @Todo: Potential problem with colors when displaying long description of
 * 		  commands. The same problem also exists in the help methods for the
 * 		  other modules. The problem is that long descriptions are multiline
 * 		  and currently embed the command prompt at linebreak. This will
 * 		  make it the same color as the help text even if the prompt color
 * 		  is changed in the Color class.
 */
void BaseTxtUi::CommandHelp(const Command &command) const {
	// display long help on specific command
	if (command.IsValid(1)) {
		Command help_command("null_command");
		if (Find(help_command, command.GetWord(1)))
			Message(help_command.LongDescription());
		else {
			/* since the command wasn't found here, check the other subsystems;
			 * if not found there either, display error message */
			if (arch_ui && arch_ui->Find(help_command, command.GetWord(1)) ||
				comm_ui && comm_ui->Find(help_command, command.GetWord(1))) {
				;
			} else {
				Error("command [" + command.GetWord(1) + "] not found");
			}
		}

	// display brief help on all commands
	} else {
		Message("--[ General commands ]--");
		
		for (CommandTable::const_command_iterator command_i =
			 command_table.begin(); command_i != command_table.end();
			 ++command_i) {
			Message((*command_i)->ShortDescription());
		}
	}
}


/**
 * Fallback method for all commands that are in the command table but not
 * implemented.
 */
void BaseTxtUi::CommandUnknown() const {
	Error("unimplemented command");
}


/**
 * Display message.
 */
void BaseTxtUi::Message(const std::string &message) const {
	std::cout << Color::Prompt << "] " << Color::Headline << message<<std::endl;
}


/**
 * Display a syntax error message.
 *
 * @param reason				description of error
 */
void BaseTxtUi::SyntaxError(const std::string &reason) const {
	std::cout << Color::Prompt << "] "
			  << Color::Error << "syntax error: " << reason << std::endl;
}


/**
 * Display an error message.
 *
 * @param reason				description of error
 */
void BaseTxtUi::Error(const std::string &reason) const {
	std::cout << Color::Prompt << "] " << Color::Error << reason << std::endl;
}
