#include "Dev.h"

namespace cry {

	/**
	* Hashes the input string and stores the result in given 32 byte array,
	* overwriting anything that was there before
	*/
	void sha256(std::string input, byte hash[32]);

}