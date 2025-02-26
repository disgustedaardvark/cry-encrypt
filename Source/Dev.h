#pragma once
#include <cstdint>
#include <string>
#define forever while(1)
#define until(X) while(!(X))
typedef unsigned char byte;
typedef unsigned long long longest;
typedef uint32_t int32;
#define DEV_MODE 0
#define SHELL_MODE 0
#if DEV_MODE==1
#define dev(X) X;
#define COMMA ,
#else
#define dev(X) ;
#endif
// version is a (little endian) integer
#define VERSION ((BRANCH_VERSION << 24) + (MAJOR_VERSION << 16) + (MINOR_VERSION << 8) + PATCH_VERSION)
#define BRANCH_VERSION 0x1 // these go from 0-ff
#define MAJOR_VERSION 0x0
#define MINOR_VERSION 0x0
#define PATCH_VERSION 0x0
#define VERSION_SIZE 4 // the number of individual numbers (bytes) that make up a version
// if off, will just parse the files with no cryptography (for dev)
#define ENCRYPTION 1
namespace cry {
	/**
	* Returns the version as a human readable string like
	* 1.14.3.243
	*/
	std::string version_as_string(int32 version = VERSION);
	/**
	* Loads the version as an array of 4 bytes, with branch at 0 and patch at 3
	*/
	void version_as_byte_array(byte bytes[VERSION_SIZE], int32 version = VERSION);

	// this is a ridiculous solution
	dev(typedef enum PrintMode { BINARY COMMA HEX COMMA ASCII } PrintMode);
}