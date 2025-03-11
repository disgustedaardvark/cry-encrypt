#include "Scrambler.h"
#include "Encryption.h"
#include "Hash.h"
#include <fstream>
#include <iostream>

namespace cry {

	Scrambler::Scrambler(std::string key) {
		// hash the key
		key += key; // double up the key -- this will [should] produce a distinct hash from normal password
		byte hash[32] = { 0 };
		sha256(key, hash);
		// convert to a full integer
		int256 generated_bytes(hash);
		longest hash1 = generated_bytes.as_long();
		longest hash2 = generated_bytes.as_long_from(8);

		// make a random generator using hash data as a seed
		random = Pseudorandom(hash1 >> 26, hash2 >> 35);
	}

	void Scrambler::scramble(std::string file_path) {
		// open access to the file, to read and write
		std::fstream file;
		file.open(file_path, std::ios::binary | std::ios::in | std::ios::out);
		if (!file) {
			// TODO error handling
			std::cerr << "Couldn't open file to scramble " << file_path << std::endl;
			file.close();
			return;
		}

		// start swapping from first byte in file body (do not touch the header)
		longest pos = BODY;
		// the two bytes that may be swapped each iter
		char byte1;
		char byte2;

		// while there is still more file to read
		while (file) {
			// read a byte 
			file.seekg(pos);
			file.read(&byte1, 1);
			// and another
			if (file) {
				file.seekg(pos + 1);
				file.read(&byte2, 1);
			}
			else break;

			// generate a pseudorandom number for a 50/50 chance
			if (random.next().is_even()) {
				// swap the two bytes in the file
				file.seekg(pos);
				file.write(&byte2, 1);
				file.seekg(pos + 1);
				file.write(&byte1, 1);
			}

			// shift the positions forward to next two bytes
			pos += 2;
		}
		// file has been scrambled
		file.close();
	}

}