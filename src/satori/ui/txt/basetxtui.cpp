
#include <QCoreApplication>
#include <QDir>
#include "basetxtui.h"


/**
 * This is the command loop that reads and executes user commands.
 */
void BaseTxtUI::Idle() {
	std::cout << "] Satori version 0.2\n] Welcome\n]\n";

	//plugins_dir = QDir(QApplication::instance()->applicationDirPath());

	//arch_ui = boost::shared_ptr<TxtUIInterface>(new AVRTxtUI);

	while (true) {
		// get user input
		std::cout << ": ";
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
			std::cout << "bad_cast exception: " << e.what() << std::endl;
			exit(1);
		} catch (std::logic_error e) {
			std::cout << "logic_error exception: " << e.what() << std::endl;
			exit(1);
		} catch (...) {
			std::cout << "unhandled exception" << std::endl;
			exit(1);
		}
	}
}


/**
 *
 */
bool BaseTxtUI::Find(Command &command, const std::string &command_name) const{
	return command_table.Find(command, command_name);
}


/**
 * Execute the command specified by the @a command object.
 *
 * @param command		command and its arguments
 */
void BaseTxtUI::Exec(const Command &command) {
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
void BaseTxtUI::CommandQuit() const {
	QCoreApplication::exit(0);		// stop event processing
}


/**
 *
 */
void BaseTxtUI::CommandShowModule() const {
	QDir module_dir = QDir(QCoreApplication::applicationDirPath()+"/../modules");
	if (!module_dir.exists()) {
		std::cout << "] can't find module directory\n";
		return;
	}

	/// @bug It could happen that arch dir does not exist and then the next
	///		 if statement will search for comm dir in the wrong place.
	///		 Also, we should probably output error if module dirs do not
	///		 exist.

	// display all working architecture modules
	if (module_dir.cd(tr("arch"))) {
		std::cout << "] --[ Architecture modules ]--\n";
		foreach (QString module_name, module_dir.entryList(QDir::Dirs)) {
			QPluginLoader loader(module_dir.absoluteFilePath(
						module_name+"/lib"+module_name+".so"));
			QObject *module = loader.instance();
			if (module) {
				TxtUIInterface *mod_if = qobject_cast<TxtUIInterface *>(module);
				CommUser *commu_if = qobject_cast<CommUser *>(module);
				if (mod_if && commu_if)
					std::cout << "]   " << module_name.toStdString() << "\n";

				if (!loader.unload())
					std::cout << "can't unload module\n";
			}
		}
	}

	// display all working communication modules
	if (module_dir.cd(tr("../comm"))) {
		std::cout << "] --[ Communication modules ]--\n";
		foreach (QString module_name, module_dir.entryList(QDir::Dirs)) {
			QPluginLoader loader(module_dir.absoluteFilePath(
						module_name+"/lib"+module_name+".so"));
			QObject *module = loader.instance();
			if (module) {
				TxtUIInterface *mod_if = qobject_cast<TxtUIInterface *>(module);
				CommProvider *commp_if = qobject_cast<CommProvider *>(module);
				if (mod_if && commp_if)
					std::cout << "]   " << module_name.toStdString() << "\n";

				if (!loader.unload())
					std::cout << "can't unload module\n";
			}
		}
	}
}


/**
 *
 */
void BaseTxtUI::CommandLoadModule(const Command &command) {
	const QString module_name(command.GetWord(1).c_str());
	bool loaded = false;

	// try loading an architecture module
	QDir modules_dir;
	modules_dir.setPath(QCoreApplication::applicationDirPath()+
											"/../modules/arch");
	if (!modules_dir.exists()) {
		std::cout << "] can't find module directory\n";
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
				// load the arch module if it implements both these interfaces
				arch_ui = qobject_cast<TxtUIInterface *>(module);
				comm_user = qobject_cast<CommUser *>(module);
				if (arch_ui && comm_user) {
					loaded = true;
					std::cout << "] loading architecture module: "
						 	  << module_name.toStdString() << "\n";

					// if a comm module is already loaded then we get it's Comm
					// object and pass it to the newly loaded arch module
					if (comm_provider) {
						std::cout << "] setting comm\n";
						comm_user->SetComm(comm_provider->GetComm());
					}

					/// @todo Replace this with proper device loading
					if (command.IsValid(1)) {
						Command cmd("null_command");
						arch_ui->Find(cmd, "device");
						cmd.ParseArguments("device " + command.GetWord(1));
						arch_ui->Exec(cmd);
					}
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
		std::cout << "] can't find module directory\n";
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
				comm_ui = qobject_cast<TxtUIInterface *>(module);
				comm_provider = qobject_cast<CommProvider *>(module);
				
				if (comm_ui && comm_provider) {
					loaded = true;
					std::cout << "] loading communication module: "
							  << module_name.toStdString() << "\n";

					// if an arch module is already loaded then we set it's Comm
					// object with the one from the newly loaded comm module
					if (comm_user) {
						std::cout << "] setting comm 2\n";
						comm_user->SetComm(comm_provider->GetComm());
					}
				} else {
					comm_ui = NULL;
					comm_provider = NULL;
				}
			}
		}
	}

	if (!loaded)
		std::cout << "] module not found\n";
}


/**
 * Clear screen.
 * 
 * @Todo If possible, find out the screen geometry at runtime (number of rows),
 * for more precise screen clearing.
 */
void BaseTxtUI::CommandClearScreen() const {
	std::cout << "]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n"
				 "]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n"
				 "]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n]\n";
}


/**
 * Display help.
 *
 * @param command				Command object holding optional command name.
 */
void BaseTxtUI::CommandHelp(const Command &command) const {
	// display long help on specific command
	if (command.IsValid(1)) {
		Command help_command("null_command");
		if (Find(help_command, command.GetWord(1)))
			std::cout << help_command.LongDescription();
		else {
			// display error only if command wasn't found in any subsystem
			if ((!arch_ui && !comm_ui) ||
				(arch_ui && !arch_ui->Find(help_command, command.GetWord(1))) ||
				(comm_ui && !comm_ui->Find(help_command, command.GetWord(1)))) {
				std::cout << "] command not found\n";
			}
		}

	// display brief help on all commands
	} else {
		std::cout << "] --[ General commands ]--\n";

		for (CommandTable::const_command_iterator command_i =
			 command_table.begin(); command_i != command_table.end();
			 ++command_i) {
			std::cout << (*command_i)->ShortDescription();
		}
	}
}


/**
 * Fallback method for all commands that are in the command table but not
 * implemented.
 */
void BaseTxtUI::CommandUnknown() const {
	std::cout << "] unimplemented command" << std::endl;
}


/**
 * Display a syntax error message.
 *
 * @param reason				short description of error
 */
void BaseTxtUI::SyntaxError(const std::string &reason) const {
	std::cout << "] syntax error: " << reason << std::endl;
}
