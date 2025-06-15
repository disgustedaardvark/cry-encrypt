#pragma once
#include <string>
#include "WinBase.h"

// chars that are not allowed in windows file paths
#define ILLEGAL_CHARS "<>:\"/|?*"
// the file extension of an encrypted file
#define FILE_EXTENSION ".cry"
// how many chars, including dot, in file extension
#define FILE_EXTENSION_CHARS 4

using namespace std;

namespace cry {

	enum ProcessingMode {
		NONE, ENCRYPTING, DECRYPTING, SCRAMBLING
	};

	/**
	* Stores information about the input and output files we are processing
	*/
	class FileInfo {
	private:
		string input_path;
		string output_path;
		string file_in_name;
		string file_in_extension;
		string file_out_name;
		string file_out_extension;
		ProcessingMode processing_mode;

		/**
		* Returns true if the file at the specified path exists in the file system
		*/
		bool file_exists(string path);

		/**
		* Preprocesses a path to remove characters that will piss off Windows
		*/
		void preprocess_path(string& path);

		void set_input_path(string path);
		/**
		* ASSUMES CORRECT FORMATTING/ acceptable path legality
		*/
		void set_output_path(string name, string extension);

	public:
		/**
		* Matches the given input file path to an output based on mode
		* (e.g. encrypting is matched to a .cry output)
		* If decrypting a file, you should update the fileinfo output file once the target extension has been decrypted
		*/
		FileInfo(string input_file, ProcessingMode processing_mode);

		FileInfo();

		/**
		* Returns true if the input file specified by path exists in the file system
		*/
		bool input_file_exists();

		/**
		* Returns true if the output file specified by path already exists in the file system
		*/
		bool output_file_exists();

		/**
		* For decryption purposes - once we've figured out what we are decrypting to, set the extension
		*/
		void set_output_extension(string extension);

		string get_input_file_path();
		string get_output_file_path();
		string get_input_extension();

	};


	

}