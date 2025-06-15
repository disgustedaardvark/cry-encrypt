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
		// flags (e.g. 'h') and their associated value
		map<char, string> flags;

	public:
		// provide the argvs from calling the program
		Parser(char* args[], int count);
		/**
		* Given empty fileinfo, populate it from the command line args.
		* Returns true if successful, false if command was invalid.
		*/
		bool parse(FileInfo* file_info);

	};

}