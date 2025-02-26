#pragma once
#include "Pseudorandom.h"

namespace cry {
	/**
	* Responsible for post-processing an encrypted file (or pre-processing when decrypting) to 
	* obfuscate the data and make hashkey identification less probable.
	* NOTE: this currently cannot confirm if a key is correct, so may make a file unrecoverable.
	*/
	class Scrambler {
	private:
		Pseudorandom random;
	public:
		Scrambler(std::string key);
		/**
		* Scrambles the bytes of the given file pseudorandomly such that a scramble operation is equivalent 
		* to an unscramble operation ONLY when this class is reinstantiated with the same key in between.
		* 
		* note: assumes file doesn't have more than 2^64 bytes.
		*/
		void scramble(std::string file_path);
	};
}