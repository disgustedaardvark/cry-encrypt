#include "Parser.h"

namespace cry {

	Parser::Parser(char* args[], int count) {
		// turn args into c++ strings:
		for (int i = 0; i < count; i++) {
			char* argument = args[i];
			if (argument[0] == '-') {
				// if this argument is a flag, check what
			}
			else {

				arguments.push_back((string) argument);
			}
			full_command.push_back((string) argument)
		}
	}

	FileInfo Parser::parse() {

	}

}