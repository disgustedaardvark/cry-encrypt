#pragma once
#include <map>
#include <vector>
#include <string>
#include "FileInfo.h"

using namespace std;

namespace cry {
	/**
	* For the parsing of command line arguments to Encryption/Scrambler instructions
	*/
	class Parser {
	private:
		// all the arguments of this command
		vector<string> full_command;
		// the non-flag arguments
		vector<string> arguments;
		// flags (e.g. 'h') and their associated value, which may be null
		map<char, string> flags;
		// map between possible flags and whether that flag takes an argument
		static map<char, bool> flag_args;

	public:
		Parser(char* args[], int count);

		FileInfo parse();

	};

	map<char, bool> Parser::flag_args;

}