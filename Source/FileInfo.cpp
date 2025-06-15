#include "FileInfo.h"
#include <algorithm>

namespace cry {

	bool FileInfo::file_exists(string path) {
		return GetFileAttributes(wstring(path.begin(), path.end()).c_str()) == INVALID_FILE_ATTRIBUTES;
	}

	bool FileInfo::input_file_exists() {
		return this->file_exists(input_path);
	}
	bool FileInfo::output_file_exists() {
		return this->file_exists(output_path);
	}

	void FileInfo::preprocess_path(string& path) {
		// replace the path directory separators with the expected \ char
		replace(path.begin(), path.end(), '/', '\\');
		// then remove any illegal chars from the path
		string illegal_chars = ILLEGAL_CHARS;
		path.erase(remove_if(path.begin(), path.end(), 
			[=](char c) { return illegal_chars.find(c) == string::npos; }), path.end());
	}

	void FileInfo::set_output_extension(string extension) {
		set_output_path(file_out_name, extension);
	}

	void FileInfo::set_output_path(string name, string extension) {
		output_path = name + extension;
		file_out_name = name;
		file_out_extension = extension;
	}


	void FileInfo::set_input_path(string path) {
		// first, preprocess the path to make it legal
		preprocess_path(path);
		// TODO test if old path and new path match and error if not
		input_path = path;
		
		// find the position of the final dot
		// two cases: there is a file extension or there isn't
		int last_dot = input_path.find_last_of('.');
		int last_slash = input_path.find_last_of('\\');
		// check that there is a . and that it isn't before a \ (i.e. part of a folder name)
		bool has_file_extension = last_dot > last_slash && last_dot != string::npos;
		// select the input file extension (either nothing or e.g. .txt)
		file_in_name = has_file_extension ? input_path.substr(0, last_dot) : input_path;
		file_in_extension = has_file_extension ? input_path.substr(last_dot) : "";
	}

	FileInfo::FileInfo(string input_file, ProcessingMode processing_mode) {
		set_input_path(input_file);
		// infer output from context:
		if (processing_mode == ENCRYPTING) {
			// always encrypt to a cry file
			set_output_path(file_in_name, FILE_EXTENSION);
		} else if (processing_mode == DECRYPTING) {
			// we don't know what the file extension to decrypt to is yet
			set_output_path(file_in_name, "");
		} else if (processing_mode == SCRAMBLING) {
			// scrambling happens in-place in the file
			set_output_path(file_in_name, file_in_extension);
		}
	}

	FileInfo::FileInfo() {
		// default constructor
		set_input_path("");
		set_output_path("", "");
		processing_mode = NONE;
		overwrite_without_asking = false;
		key_hint = NO_KEY_HINT;
	}

	string FileInfo::get_input_file_path() {
		return input_path;
	}
	string FileInfo::get_output_file_path() {
		return output_path;
	}
	string FileInfo::get_input_extension() {
		return file_in_extension;
	}

	void FileInfo::set_create_hint_mode(string hint) {
		this->key_hint = hint;
	}

	void FileInfo::set_overwrite_prompt_mode(bool new_value) {
		this->overwrite_without_asking = new_value;
	}
}