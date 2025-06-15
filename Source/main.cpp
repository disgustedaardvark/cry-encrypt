#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "Encryption.h"

using namespace std;
using namespace cry;

#define DRIVER 1

std::string currentWorkingDirectory = "";

#if DRIVER==1
int main(int argc, char* argv[]) {

	// SEE parser instead

	// process arguments:
	vector<string> arguments;
	for (int i = 0; i < argc; i++) {
		// extract flags:
		if ()
		arguments.push_back((string) argv[i]);
	}
	

	// first, utility commands
	switch (argc) {
	case 1:
		// if there are no arguments, error.
		cerr << "No arguments specified" << endl;
		return 1;
	case 2:
		// if there is one argument, command is legal for 'help' and 'version'
		if (arguments.at(1) == "help") {
			// TODO
			return 0;
		}
		else if (arguments.at(1) == "version") {
			cout << "Cry version is " << version_as_string() << endl;
			return 0;
		}
		else {
			cerr << "Malformed command" << endl;
			return 1;
		}
	default:
	}

	// determine mode of operation
	ProcessingMode mode = NONE;
	// extract the arguments
	string key;
	string file_names;
	// select desired mode 
	if (arguments.at(1) == "scramble") {
		mode = SCRAMBLING;
		key = arguments.at(2);
		key = arguments.at(2);
	}
	else if (arguments.at(1) == "encrypt") {
		mode = ENCRYPTING;
	}
	else if (arguments.at(1) == "decrypt") {
		mode = DECRYPTING;
	}
	else {
		// otherwise, assume this is the shorthand command:
		// $ cry filename key
		// determine whether to encrypt or decrypt based on whether the supplied file ends in .cry
		string arg1 = arguments.at(1);
		if ((arg1.length() >= FILE_EXTENSION_CHARS) 
			&& (arg1.substr(arg1.length() - FILE_EXTENSION_CHARS, arg1.length()) == FILE_EXTENSION)) {
				// file is a CRY file, so decrypt
			mode = DECRYPTING;
		}
		else {
			mode = ENCRYPTING;
		}
		 
		// so check file exists
		if (!fileExists(arguments.at(1))) {
			// file does not exist/inaccessible
			cerr << "Argument #1: " << arguments.at(1) << " is not an accessible file; cannot process" << endl;
			return 1;
		}
		if (argc < 3) {
			// no key has been specified
			cerr << "Cannot process " << arguments.at(1) << " without a encryption/decryption key" << endl;
			return 1;
		}

		// encrypt or decrypt the file TODO
	}
	
}
/*
// TODO remove this method
int test() {

	// if input_path is a .cry file, decrypt it and store in a file based on the header data
	// otherwise, encrypt it the file and store in a corresponding .cry file
	string input_path;
	// the file names are without the final .txt / .cry / .etc
	// the extensions may be "" or ".txt" / ".cry" / ".etc"
	string file_in_name;
	string file_in_extension;
	string file_out_name;
	string file_out_extension;
	string key;

#if SHELL_MODE==1
	// use first arg as target file if in shell mode [TODO test arg count instead, remove shellmode]
	if (argc < 2) {
		throw runtime_error("Specify file name as argument to encrypt");
	}

	string file_in_name = argv[1];
#else
	// TODO line input here instead
	input_path = "C:\\Users\\samue\\Documents\\Coding\\Encryption\\Cry\\TestData\\encrypt_test.txt";
#endif
	key = "password2";

	// preprocess the input path -- all forward slashes should be backslashes
	std::replace(input_path.begin(), input_path.end(), '/', '\\');
	std::cout << input_path << std::endl;
	std::cout << "Cry Version: " << version_as_string() << std::endl;

	// get the file extension of the input file:
	// find the final . if there is one
	int last_dot = input_path.find_last_of('.');
	int last_slash = input_path.find_last_of('\\');
	// check that there is a . and that it isn't before a \ (i.e. part of a folder name)
	bool has_file_extension = last_dot > last_slash && last_dot != string::npos;
	// select the input file extension (either nothing or e.g. .txt)
	file_in_name = has_file_extension ? input_path.substr(0, last_dot) : input_path;
	file_in_extension = has_file_extension ? input_path.substr(last_dot) : "";
	file_out_name = file_in_name; // by default, the path header/file name should match initial file
	// if we're decrypting, leave file extension blank for decryptor to resolve when it's started writing
	// if we're encrypting, encrypt to .cry
	file_out_extension = file_in_extension == FILE_EXTENSION ? "" : FILE_EXTENSION;

	// make the encryption manager
	Encryption encryption(key, file_in_name, file_in_extension, file_out_name, file_out_extension);

	// check the file extension type -- if it's .cry, we need to decrypt
	if (file_in_extension == FILE_EXTENSION) {
		// decrypt the given file (it's a .cry)
		encryption.decrypt();
	}
	else {
		// encrypt the given file
		encryption.encrypt();
	}

	// remove all copies of encryption key from memory, just to be sure :)
	key.erase(0);
	encryption.destroy_key();

	return 0;
}*/
#else
// Here a test main method for other purposes
int main() {

	// test the hashing system
	byte hash[32] = { 0 };
	sha256("", hash);
	print_hash(hash);
	sha256("test", hash);
	print_hash(hash);
	sha256("verylonghash000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", hash);
	print_hash(hash);

	return 0;
}
#endif