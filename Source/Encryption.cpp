#include "Encryption.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <unordered_map>

namespace cry {

	Encryption::Encryption(std::string key, std::string file_in, std::string file_in_extension,
		std::string file_out, std::string file_out_extension) {
		
		this->key = key;
		generate_hash_key(key, key_hash);

		// seed the pseudorandom generator
		dev(std::cout << "Initialising pseudorandom..." << std::endl);
		// make seed from key
		longest pseudoseed = 0;
		for (int i = 0; i < sizeof(longest); i++) {
			pseudoseed |= (((longest) key_hash[i]) << (i*8));
		}
		// leave some space at the top of the pseudoseed to avoid overflows when we're about to do silly transformations
		pseudoseed = pseudoseed xor (((longest)0xa0) << (8 * (sizeof(longest) - 1)));
		// the pseudoseed should not be dependent only on the hash
		// so that if attackers calculate the hash but do not the key,
		// they cannot derive the pseudorandom transformation from the original hashkey
		// to achieve this, transform the pseudoseed based on the key 1) length 2) ascii total
		pseudoseed += (key.length() * 50000);
		for (int i = 0; i < key.length(); i++) {
			pseudoseed += (key[i] * 3123); // arbitrary constant
		} // these numbers are large so they have a significant impact

		dev(std::cout << "Generated pseudoseed " << pseudoseed << std::endl);
		// just in case the key hashed to a really low value (which would be bad for the security
		// of the generation), add a min value
		pseudoseed += ((longest) (0x3)) << 50;
		// initialise the generators
		random_encryptor = Pseudorandom(pseudoseed >> 32, pseudoseed >> 40);
		// transform the seed some more for the other generator
		/*pseudoseed = ~pseudoseed;
		pseudoseed += pseudoseed % 3000000;
		pseudoseed += 1000000000000;
		pseudoseed = pseudoseed + (((longest)(0x03)) << (8 * (sizeof(longest) - 1))); // keep the seed high
		random_scrambler = Pseudorandom(pseudoseed >> 32, pseudoseed >> 40);*/
		dev(std::cout << "Pseudorandom initialised." << std::endl);

		// file paths
		this->file_in = file_in;
		this->file_extension_in = file_in_extension;
		this->file_out = file_out;
		this->file_extension_out = file_out_extension;
		// jump to start of file
		this->reset_cursor();
		// clear the buffer
		this->reset_buffer();

		bytes_written = 0;
	}

	void Encryption::generate_hash_key(std::string key, byte resulting_key_hash[BUFFER_SIZE]) {
		// hash the input key via a cryptographically secure method to 32 bytes
		sha256(key, resulting_key_hash);

		dev(std::cout << "HASHED 32 BYTE KEY IS ";
			for (int i = 0; i < BUFFER_SIZE; i++) std::cout << std::hex << (int)resulting_key_hash[i];
			std::cout << std::endl);
	}

	void Encryption::generate_hash_key(byte key_hash[BUFFER_SIZE], byte resulting_key_hash[BUFFER_SIZE]) {
		// (re)hash the given bytes with sha256
		sha256(key_hash, 32, resulting_key_hash);
	}

	void Encryption::reset_cursor() {
		this->cursor = 0;
		this->buffer_source = 0;
	}

	void Encryption::write_int(int32 n) {
		dev(std::cout << "Writing integer: " << n << std::endl);
		// write in little-endian format (so the least significant byte first)
		byte bytes[sizeof(n)];
		for (int i = 0; i < sizeof(n); i++) {
			bytes[i] = (byte) ((n >> (i * 8)) & 255);
		}
		// if all bytes can be processed succesfully, write them to the outfile
		for (int i = 0; i < sizeof(n); i++) {
			output << bytes[i];
		}

		bytes_written += sizeof(n);
	}

	int32 Encryption::read_int() {
		const int size = sizeof(int32);
		// read 4 bytes into a buffer
		char bytes[size];
		file.seekg(cursor);
		file.read(bytes, size);
		// convert them to a (little endian) integer
		int32 word = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];

		if (file.eof()) {
			// TODO handle this
		}
		// move cursor forward
		cursor += size;

		return word;
	}
	// TODO move to EncryptionBuffer object
	void Encryption::reset_buffer() {
		// clear the buffer
		for (int i = 0; i < BUFFER_SIZE && !(buffer[i] = 0); i++);
	}

#if DEV_MODE==1
	void Encryption::print_buffer(PrintMode mode) {
		switch (mode) {
		case HEX:
			// format this line in hex pairs
			for (int i = 0; i < BUFFER_SIZE; i++) {
				// output each byte as two hex digits
				printf("%02x", (int)buffer[i]);
			}
			break;
		case BINARY:
			for (int i = 0; i < BUFFER_SIZE; i++) {
				// output each bit as 1/0
				for (int j = 7; j >= 0; j--) {
					// select each bit individually
					char bit = (buffer[i] >> j) & 1;
					// convert to ascii 1/0
					std::cout << (byte)(bit + 48);
				}
			}
			break;
		case ASCII:
			// render each character as is (should be nonsense if encrypted)
			for (int i = 0; i < BUFFER_SIZE; i++) {
				std::cout << buffer[i];
			}
			break;
		}
		std::cout << std::endl;
	}
#endif

	/*Encryption::~Encryption() {

	}*/

	bool Encryption::fill_buffer(int buffer_subsize) {
		// don't allow buffer overflow
		if (buffer_subsize > BUFFER_SIZE) {
			std::cout << "Risked buffer overflow while reading file..." << std::endl;
			buffer_subsize = BUFFER_SIZE;
		}

		dev( // Output position of Cursor
			std::cout << "Cursor: " << cursor << std::endl;
			);
		// track which char this buffer load started at
		buffer_source = cursor;
		// jump to cursor pos in file and read into buffer
		file.seekg(cursor);
		file.read(buffer, buffer_subsize);

		if (!file) {
			// managed to read this many bytes (probably reached end of file)
			std::streamsize bytes_read = file.gcount();
			// advance the cursor
			cursor += bytes_read;
			dev(std::cout << "Finished reading file." << std::endl);
			// fill the remaining block with 0s
			for (std::streamsize i = bytes_read; (i < buffer_subsize) && !(buffer[i] = 0); i++);
			// return true -- we reached the end of the file
			return true;
		}
		else {
			cursor += buffer_subsize;
		}

		return false;
	}

	bool Encryption::fill_buffer() {
		// by default, fill the whole buffer
		return fill_buffer(BUFFER_SIZE);
	}

	void Encryption::write_buffer() {
		// write each sequential byte in buffer to the file out
		// if the file reading has ended, write up until the cursor
		// so we don't write trailing 0s that are in the buffer (because it couldn't be filled)
		long long write_until =  cursor - buffer_source;
		// (cursor is not affected by writing, only reading)
		for (long long i = 0; i < write_until; i++) {
			output << buffer[i];
			bytes_written++;
		}
	}

	void Encryption::write_full_buffer() {
		// write the whole buffer to the file, no checks
		for (int i = 0; i < BUFFER_SIZE; i++) {
			output << buffer[i];
		}
		bytes_written += BUFFER_SIZE;
	}

	void Encryption::crypt_buffer() {
#if ENCRYPTION==1
		// switch to the next key hash in the sequence:
		
		// first do some pseudorandom transformations on the key
		// this will prevent hash cycling
		int256 next = random_encryptor.next();
		byte* random_bytes = next.get_byte_array();
		for (int i = 0; i < BI_INT_BYTE_SIZE; i++) {
			// loop through the bytes in the key
			// add the random offset to the key 
			random_bytes[i] = random_bytes[i] xor key_hash[i];
		}
		// make the new key
		generate_hash_key(random_bytes, key_hash);

		
		dev(std::cout << "Encrypting plainbytes: " << std::hex);
		// xor all the bits to encrypt with this key
		for (int i = 0; i < BUFFER_SIZE; i++) {
			dev(std::cout << (int) buffer[i]);
			buffer[i] = buffer[i] xor key_hash[i];
		}
		dev(std::cout << std::endl);
#endif
	}

	bool Encryption::add_to_buffer(char* str) {
		// clear the buffer
		reset_buffer();
		for (int i = 0; (i < BUFFER_SIZE - 1) && (str[i] != '\0'); i++) {
			// write string to buffer while there is any of both
			buffer[i] = str[i];
		}
		// if the penultimate char is not \0 and the following char in str is not \0, not all the string 
		// was written to the buffer, so return false
		return !(buffer[BUFFER_SIZE - 1] != '\0' && str[BUFFER_SIZE] != '\0');
	}

	bool Encryption::process_next(Mode mode) {

		// fill up the buffer with the next block of data to de/encrypt
		bool done = fill_buffer();

		// dev: output the contents of the buffer in hex
		dev(std::cout << "\t";
			print_buffer(ASCII));

		// do any symmetric then asymmetric encryption/decryption on the buffer
		switch (mode) {
		case ENCRYPT:
			encrypt_buffer(); break;
		case DECRYPT:
			decrypt_buffer(); break;
		}
		// write processed data to output file
		write_buffer();

		return done;
	}

	void Encryption::encrypt() {
		// open up the file to encrypt
		file.open(file_in + file_extension_in, std::ios::binary);
		// remove old output file and create a new one to store encrypted data
		output.open(file_out + file_extension_out, std::ios::binary | 
			std::fstream::in | std::fstream::out | std::fstream::trunc);

		std::cout << "Encrypting file " << file_in << file_extension_in << " to " << file_out << file_extension_out << std::endl;

		// error, stop
		if (!file || !output) {
			std::cerr << "Couldn't open file" << std::endl;
			return;
		}

		// reset the cursor and buffer
		this->reset_buffer();
		this->reset_cursor();

		// write the header:
		// first, the version info
		write_int(VERSION);
		// encrypt the Key Test bytes (used to confirm a key is correct before further decrypting)
		add_to_buffer((char*) KEY_TEST);
		encrypt_buffer();
		write_full_buffer();

		// next write the file extension
		reset_buffer();
		for (int i = 0; i < BUFFER_SIZE && i < file_extension_in.length(); i++) { // TODO warn about fileext length limit
			// loop through the characters in the file extension and add them to the buffer
			buffer[i] = file_extension_in[i];
		}
		encrypt_buffer();
		write_full_buffer();

		this->reset_buffer();

		// keep encrypting until nothing is left
		until (this->process_next(ENCRYPT));

		// then continuously scramble bytes until done
		//this->scramble();

		std::cout << "Encryption complete" << std::endl;

		// always close the files once done!
		file.close();
		output.close();
	}

	void Encryption::encrypt_buffer() {
#if ENCRYPTION == 1
		// do symmetric encryption on the buffer
		crypt_buffer();
#endif
	}

	void Encryption::decrypt_buffer() {
#if ENCRYPTION == 1
		// do symmetric decryption on the buffer
		crypt_buffer();
#endif
	}

	void Encryption::decrypt() {
		// open up the file to decrypt
		file.open(file_in + file_extension_in, std::ios::binary);
		
		// error getting output file, stop
		if (!file) {
			std::cerr << "Couldn't open input file" << std::endl;
			return;
		}

		// read the header:
		// first an integer (the version of Cry that encrypted this)
		int32 encryption_version = read_int();
		std::cout << "File was encrypted with version " << version_as_string(encryption_version) << std::endl;
		// then a buffer: the Key tester
		if (fill_buffer()) {
			// TODO and restructure this whole thing
		}
		else {
			decrypt_buffer();
			std::cout << "Decrypted Key Test as : " << buffer << std::endl;
			if (strcmp(buffer, KEY_TEST) == 0) {
				// strings are the same! decryption key is correct, proceed
				std::cout << "Decryption Key verified." << std::endl;
			}
			else {
				// TODO exit
				std::cout << "Incorrect decryption key, stopping. " << std::endl;
				file.close(); output.close();
				return;
			}
		}

		// read the next buffer's worth of data to figure out the output file extension
		if (fill_buffer()) {
			dev(std::cout << "File empty? Buffer empty after reading file extension.");
			// TODO handle this throw
		}
		else {
			// extension can be max BUFFER_SIZE - 2 characters long (.extension\0)
			// decrypt it first
			decrypt_buffer();
			for (int i = 0; i < BUFFER_SIZE && buffer[i] != '\0'; i++) file_extension_out += buffer[i];
		}

		// TODO y/n overwrite are you sure?
		// remove old output file and create a new one under correct file extension to store decrypted data
		std::cout << file_out << file_extension_out << std::endl;
		output.open(file_out + file_extension_out, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

		// error getting output file, stop
		if (!output) {
			std::cerr << "Couldn't open output file" << std::endl;
			file.close(); output.close();
			return;
		}

		// unscramble bytes in file body
		//this->scramble();
		// jump back to the right part of the file to decrypt
		//file.seekg(BODY);

		// decrypt file body:
		// keep decrypting chunk by chunk until done
		until (this->process_next(DECRYPT));

		std::cout << "Decryption complete" << std::endl;
		
		// always close the files once done!
		file.close();
		output.close();

	}

	void Encryption::destroy_key() {
		key.erase(0);
	}
}