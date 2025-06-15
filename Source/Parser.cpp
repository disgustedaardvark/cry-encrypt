#include "Parser.h"
#include "Dev.h"
#include <iostream>

namespace cry {

	Parser::Parser(char* args[], int count) {

		// turn args into c++ strings:
		for (int i = 0; i < count; i++) {
			char* argument = args[i];
			if (argument[0] == '-') {
				// if this argument is a flag, add its argument to the map and skip over it
				string flag_argument = (string) args[i + 1];
				char flag = argument[1];
				flags[flag] = flag_argument;
				// log them as part of the whole command 
				full_command.push_back((string) argument);
				full_command.push_back((string) flag_argument);
				// we have handled the flag arg, so skip it
				i++;
			}
			else {
				// store non-flag arguments in sequence
				arguments.push_back((string) argument);
				full_command.push_back((string)argument);
			}
		}
	}

	bool Parser::parse(FileInfo* file_info) {
		ProcessingMode mode = NONE;
		vector<string> file_names;
		string key = "";

		switch (arguments.size()) {
			// where the first argument is just 'cry'
		case 0:
		case 1:
			std::cerr << "Please specify arguments. Try $cry help" << std::endl;
			return false;
		case 2:
			// allowed for help/version commands
			if (arguments.at(1) == "help") {
				print_help();
			}
			else if (arguments.at(1) == "version") {
				std::cout << "Cry Version is " << version_as_string << std::endl;
			}
			// don't continue to processing
			return false;
		case 3:
			// this is the shorthand case, $ cry filename key
			// infer if we are encrypting or decrypting based on the given file's extension
			string file_name = arguments.at(1);
			file_names.push_back(file_name);
			key = arguments.at(2);
			// check the extension
			if (file_name.substr(file_name.length() - FILE_EXTENSION_CHARS, FILE_EXTENSION_CHARS) == FILE_EXTENSION) {
				// it's a .cry, so decrypt it
				mode = DECRYPTING;
			}
			else mode = ENCRYPTING;
			break;
		case 4:
			// either $cry [encrypt, key, filename] or [decrypt ...] or [scramble ...]
			key = arguments.at(2);
			file_names.push_back(arguments.at(3));
			if (arguments.at(1) == "encrypt") mode = ENCRYPTING;
			else if (arguments.at(1) == "decrypt") mode = DECRYPTING;
			else if (arguments.at(1) == "scramble") mode = SCRAMBLING;
			else {
				std::cerr << "Error - argument " + arguments.at(1) + " is invalid." << std::endl;
				return false;
			}
			break;
			// TODO locksmith
		default:
			// when multiple encryption is desired. $cry encrypt key filename1 filename2 filename3...
			if (arguments.at(1) == "encrypt") {
				mode = ENCRYPTING;
				key = arguments.at(2);
				for (int i = 3; i < arguments.size(); i++) {
					file_names.push_back(arguments.at(i));
				}
				break;
			}
			std::cerr << "Malformed command" << std::endl;
			return false;
		}
		// TODO here: create file info object, handle multifiles etc. check files exist.
		FileInfo file;

		
		// add the password hint if there was one
		if (flags.count('h')) {
			file_info->set_create_hint_mode(flags['h']);
		}
		// set the certainty mode if it was set
		if (flags.count('c') && flags['c'] == "true") {
			file_info->set_overwrite_prompt_mode(true);
		}
	}
}