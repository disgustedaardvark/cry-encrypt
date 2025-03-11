#pragma once
#include <string>
#include <fstream>
#include "Dev.h"
#include "Pseudorandom.h"
#include "Hash.h"

#define FILE_EXTENSION ".cry"
#define KEY_TEST "AmyAkaSpentGladiatorVersionTwo"
#define HEADER 0
// where the payload (ciphertext) starts
#define BODY 0x44
#define BUFFER_SIZE 32

namespace cry {

	enum Mode {
		DECRYPT, ENCRYPT
	};

	class Encryption {

	private:
		std::string file_in;
		std::string file_out;
		std::string file_extension_in;
		std::string file_extension_out;
		std::ifstream file;
		std::fstream output;
		char buffer[BUFFER_SIZE]; // 32 bytes
		/**
		* Current char position in the READING file (in)
		*/
		long long cursor;
		/**
		* The char position which the buffer was just filled from - in the reading input file.
		* So the first byte in the buffer matches whatever is at this position in the file [assuming no EOF].
		* Only accurate to the last buffer fill from file. see fill_buffer()
		*/
		long long buffer_source;
		/**
		* The encryption/decryption key
		*/
		std::string key;
		/**
		* The hash key used to encrypt the buffer -- the same size as the buffer
		*/
		byte key_hash[BUFFER_SIZE];

		/**
		* Keep track of the size of our output file for scrambling later
		*/
		longest bytes_written;

		/**
		* Stable long-cycle random number algorithm for stream encryption
		*/
		Pseudorandom random_encryptor;

		/**
		* Turns the given key into a full hashed key of BUFFER_SIZE (32) bytes and places it into the input array buffer
		*/
		void generate_hash_key(std::string key, byte resulting_key_hash[BUFFER_SIZE]);
		/**
		* For reprocessing a hash key (cascading the key), converts the given current key into the next places it into the given buffer
		* (Does not replace the current key). It is safe for both byte[] to be the same array (an internal copy is made).
		*/
		void generate_hash_key(byte key_hash[BUFFER_SIZE], byte resulting_key_hash[BUFFER_SIZE]);
		/**
		* Read the next [32] bytes of the file into a buffer. Fill any remaining bytes (e.g. if eof) with 0.
		* Returns true if file fully read / done.
		*/
		bool fill_buffer();
		bool fill_buffer(int buffer_subsize);
		/**
		* Process the next set of bytes from the file input.
		* @param mode is decrypt/encrypt
		*/
		bool process_next(Mode mode);

		/**
		* Read a single 32-bit integer from the input file and returns it, shifting the cursor forward.
		* Assumes a little-endian integer.
		* Does not touch the buffer.
		*/
		int32 read_int();
		/**
		* Writes a single unsigned 32-bit integer to the output file.
		* Writes in little-endian format.
		* Does not touch the buffer.
		*/
		void write_int(int32 n);

		/**
		* Loads the given TERMINATED string into the buffer, up to 31 chars.
		* char 32 will always be \0 when loading string by this method, for safety.
		* Returns false if the str (would have) overflowed the buffer so it wasn't all written, true otherwise.
		*/
		bool add_to_buffer(char* str);

		/**
		* Symmetrically encrypt/decrypt the contents of the buffer (pushes forward internal state).
		* Called by decrypt/encrypt_buffer methods.
		*/
		void crypt_buffer();

		/**
		* Asymetrically decrypt the contents of the buffer, after symmetrically processing, using the stored key
		*/
		void decrypt_buffer();
		/**
		* Asymetrically encrypt the contents of the buffer, after symmetrically processing, using the stored key
		*/
		void encrypt_buffer();
		/**
		* Reset the buffer and cursor position to the start of the file
		*/
		void reset_buffer();
		/**
		* Writes the contents of the buffer to the output file. Will cut off any (trailing) data in the buffer which was not
		* read directly from the file via fill_buffer (or read_int).
		*/
		void write_buffer();

		/**
		* Writes the full contents of the buffer to the output file, regardless of what they are or where they came from.
		* Therefore is not concerned about whether they were or were not from the input file.
		*/
		void write_full_buffer();
		/**
		* Place the cursor at position 0 of the READING file, i.e. the start.
		* Also moves the buffer_source.
		*/
		void reset_cursor();
		
#if DEV_MODE==1
		/**
		* Print contents of buffer to console. For security reasons, not including in non-dev versions
		*/
		void print_buffer(PrintMode mode);
#endif

	public:
		//Encryption(std::string file_in, std::string file_out); // TODO this constructor
		Encryption(std::string key, std::string file_in, std::string file_in_extension, std::string file_out, std::string file_out_extension);
		//~Encryption();

		/*
		**/
		void encrypt();
		void decrypt();
		/**
		* overwrites the stored key in memory
		*/
		void destroy_key();
	};
}