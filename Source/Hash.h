#include "Dev.h"

namespace cry {

	/**
	* Performs bitwise rotation right on x of n places
	*/
	int32 rotate_right(int32 x, int32 n);
	/**
	* Hashes the input string and stores the result in given 32 byte array,
	* overwriting anything that was there before
	*/
	void sha256(std::string input, byte hash[32]);

}