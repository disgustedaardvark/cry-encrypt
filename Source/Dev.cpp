#include "Dev.h"
#include <iostream>

namespace cry {

	void version_as_byte_array(byte bytes[VERSION_SIZE], int32 version) {
		// separate each byte in the [presumably int] version code
		for (int i = 0; i < VERSION_SIZE; i++) {
			bytes[i] = (byte)((version >> (i * 8)) & 255);
		}
	}

	std::string version_as_string(int32 version) {
		// get the version numbers separately as bytes
		byte bytes[VERSION_SIZE];
		version_as_byte_array(bytes, version);

		// make an empty string to fill up with each version tier
		std::string version_string = "";
		// looping little-endianly
		for (int i = VERSION_SIZE - 1; i > -1; i--) {
			// add each version number and a dot
			version_string += std::to_string((int)(bytes[i]));
			version_string += '.';
		}
		// cut off the final dot
		version_string = version_string.substr(0, version_string.length() - 1);

		return version_string;
	}

	void print_help() {
		// print the help menu
		std::cout << "Help menu: coming soon" << std::endl; // TODO
	}
}