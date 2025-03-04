#include "Dev.h"

namespace cry {

	/**
	* Performs bitwise rotation right on x of n places
	*/
	int32 rotate_right(int32 x, int32 n);

	/**
	* Converts given byte[] WHICH MUST BE AT LEAST 4 BYTES LONG to a big endian unsigned integer
	*/
	int32 to_big_endian(const byte* bytes);

	/**
	* Hashes the input string and stores the result in given 32 byte array,
	* overwriting anything that was there before
	*/
	void sha256(std::string input, byte hash[32]);

	/**
	* Hashes given input (length) bytes and stores in given byte hash array
	* (see main sha256 function)
	*/
	void sha256(byte* bytes, int32 length, byte hash[32]);

	/**
	* Takes the given 256 bit hash and rehashes it (i.e. the given hash is the input) and stores it in the same array.
	*/
	void sha256(byte hash[32]);

#if DEV_MODE==1
	void print_hash(byte hash[32]);
#endif
}