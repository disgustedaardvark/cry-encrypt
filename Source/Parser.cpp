#include "Parser.h"

namespace cry {

	Parser::Parser(char* args[], int count) {

		// turn args into c++ strings:
		for (int i = 0; i < count; i++) {
			char* argument = args[i];
			if (argument[0] == '-') {
				// if this argument is a flag, add its argument to the map and skip over
				string flag_argument = (string) args[i + 1];
				flags.insert((const char) argument[1], flag_argument);
				// log them as part of the whole command 
				full_command.push_back((string) argument);
				full_command.push_back((string) flag_argument);
				i++;
			}
			else {
				// store non-flag arguments in sequence
				arguments.push_back((string) argument);
				full_command.push_back((string)argument);
			}
		}
	}

	FileInfo Parser::parse() {

	}
}