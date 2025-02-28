#include "Hash.h"
#include "BigInt.h"

namespace cry {

	void sha256(std::string input, byte hash[32]) {
		// implementation src: wikipedia https://en.wikipedia.org/wiki/SHA-2#Pseudocode

		// initialise initial hash values, which are the fractional parts of the square roots of first 8 primes
		int32 hash[8] = {
			0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 
			0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };

		// and an array of round constants (first 32 bits of fractional parts of cube rootsof first 64 primes)
		int32 k[64] = {
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

		// preprocess the hash input
		
		longest string_bit_length = input.length() * 8;
		longest zero_padding = (512 - (string_bit_length + 1 + 64) % 512);
		longest bit_length = string_bit_length + 1 + zero_padding + 64;
		// bit length is now a multiple of 512
		longest byte_length = bit_length / 8;
		// make an array to store the padded input bytes
		byte* padded_input = new byte[byte_length];
		// and fill it with the bytes of:
		// <original message> 1 <zero_padding to multiple 512> <64 bit int -- string length>
		// first copy message bytes in
		for (longest i = 0; i < input.length(); i++) {
			padded_input[i] = (byte) input[i];
		}
		// write the chunks of 1-0s
		// write the 1---- bit first
		padded_input[input.length()] = 0x80;
		for (longest i = input.length() + 1; i < byte_length - 8; i++) {
			// fill in the rest of the zeroes
			padded_input[i] = 0x00;
		}
		// and write the final length, 64 bit big endian
		int byte_of_long = 0;
		// loop backwards, writing least significant bytes first
		for (int i = byte_length - 1; i >= byte_length - 8; i--) {
			// ensure we're writing it Big-endian, so byte by byte
			// select the next byte
			byte big_endian_byte = (byte) ( (0xff) & (string_bit_length >> (byte_of_long * 8)) );
			// write it to padded input
			padded_input[i] = big_endian_byte;
			// advance position in big endian number
			byte_of_long++;
		}

		// don't leak memory
		delete padded_input;

		// break into 32-bit chunks
		// every 16 of these is 512 bits.
		int32* chunks = (int32*) padded_input;
		int chunk32_count = byte_length / 4;
		int chunk512_count = chunk32_count / 16;
		for (int i = 0; i < chunk512_count; i++) {
			// for each 512 bit chunk
			word schedule_array[64];
			// copy this chunk into the first 16 words of the array
			for (int j = 0; j < 16; j++) {
				schedule_array[j] = chunks[i * 16 + j];
			}
			// extend first 16 ints into remaining 48
			for (int j = 16; j < 64; j++) {
				word s0 = (rotate_right(schedule_array[j - 15], 7))
					xor (rotate_right(schedule_array[j - 15], 18))
					xor (rotate_right(schedule_array[j - 15], 3));
				word s1 = (rotate_right(schedule_array[j - 2], 17))
					xor (rotate_right(schedule_array[j - 2], 19))
					xor (rotate_right(schedule_array[j - 2], 10));
				schedule_array[j] = schedule_array[j - 16] + s0 + schedule_array[j - 7] + s1;
			}

			// initialise working values to current hash value
			int32 a = hash[0];
			int32 b = hash[1];
			int32 c = hash[2];
			int32 d = hash[3];
			int32 e = hash[4];
			int32 f = hash[5];
			int32 g = hash[6];
			int32 h = hash[7];

			// run compression function
			// honestly who the fuck knows what this does
			// hashing i would imagine
			for (int j = 0; j < 64; j++) {
				int32 s1 = (rotate_right(e, 6)) xor (rotate_right(e, 11)) xor (rotate_right(e, 25));
				int32 ch = (e & f) xor ((~e) & g);
				int32 temp1 = h + s1 + ch + k[j] + schedule_array[j];
				int32 s0 = (rotate_right(a, 2)) xor (rotate_right(a, 13)) xor (rotate_right(a, 22));
				int32 maj = (a & b) xor (a & c) xor (b & c);
				int32 temp2 = s0 + maj;
				h = g;
				g = f;
				f = e;
				e = d + temp1;
				d = c;
				c = b;
				b = a;
				a = temp1 + temp2;
			}
			// add compressed chunk to current hash
			hash[0] += a;
			hash[1] += b;
			hash[2] += c;
			hash[3] += d;
			hash[4] += e;
			hash[5] += f;
			hash[6] += g;
			hash[7] += h;
		}
		// final (big endian) hash is now loaded into result array
	}

	int32 rotate_right(int32 x, int32 n) {
		return x >> n | (x << (32 - n));
	}
}