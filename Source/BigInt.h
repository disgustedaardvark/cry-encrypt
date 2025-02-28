#pragma once
#include <cstdint>
#include <string>
#include <atomic>

#define BI_INT_128 128
#define BI_INT_256 256
#define BI_INT_512 512
#define BI_INT_1024 1024
#define BI_INT_2048 2048
#define BI_INT_4096 4096

// concat tokens
#define BI_CONCAT(A, B) A ## B
// expand macro inputs then concat tokens
#define BI_EVAL_CONCAT(A, B) BI_CONCAT(A, B)

// how many bits this int is
// if defined already, use whatever has been picked externally
#ifndef BI_BIG_INT_SIZE
#define BI_BIG_INT_SIZE BI_INT_256
#endif

// how many bytes this int is
#define BI_INT_BYTE_SIZE (BI_INT_SIZE >> 3)
#define BI_BIG_INT_CLASS BI_EVAL_CONCAT(int, BI_INT_SIZE)

// then convert to the internal macros
#define BI_INT_CLASS BI_BIG_INT_CLASS
#define BI_INT_SIZE BI_BIG_INT_SIZE

#define GEN_CLASS(X) X;\nX

typedef unsigned char byte;

namespace bigint {


	/**
	* For storage of large unsigned integers (e.g. used in cryptography algorithms)
	*/
	class BI_INT_CLASS {
	private:
		// stored little endian -- i.e. bytes[0] is the least significant
		byte bytes[BI_INT_BYTE_SIZE];

		/**
		* Adds two bytes x and y together, and stores the result in x.
		* Returns carry overflow as a byte.
		*/
		byte add_bytes(byte* x, byte y);

	public:
		/**
		* Empty int initialised to 0
		*/
		BI_INT_CLASS();
		/**
		* Copy constructor -- duplicates contents of byte array
		*/
		BI_INT_CLASS(const BI_INT_CLASS& other);
		BI_INT_CLASS& operator=(const BI_INT_CLASS& other); // and copy assignment
		/**
		* Empty int initialised to n, but UNSIGNED
		*/
		BI_INT_CLASS(int n);
		/**
		* Empty int initialised to n [unsigned]
		*/
		BI_INT_CLASS(unsigned long long n);
		/**
		* Make an int 256 from a corresponding 32 byte array, little endian (smallest index is least significant).
		* I.e. bytes should match this int size (e.g. 64 for int512 etc)
		*/
		BI_INT_CLASS(byte bytes[BI_INT_BYTE_SIZE]);
		/**
		* Initalises all bytes to 0 and places given byte in least significant byte
		*/
		BI_INT_CLASS(byte lowest_byte);

		/**
		* Writes given byte to the first n bytes (starting with least-significant) of this int
		*/
		BI_INT_CLASS(byte byte, unsigned int n);

		/**
		* Set this integer to store 0 unsigned
		*/
		void zero();

		/**
		* Adds the given byte to this number, as if it was the ith byte in the value (i.e. carries through).
		* E.g. to add a byte to the number as normal, call with i=0.
		* E.g. to add '512' to the number, call with i=1, b=0x02
		* I.e. i is a bit shift left for b
		*/
		void add_byte_from(int i, byte b);

		/**
		* Left shift by n, operating on internal bytes array. Only guaranteed to work for
		* 0 <= shifts < 32 (internals aiming for efficiency).
		* Shift multiple separate times to bypass this.
		*/
		BI_INT_CLASS operator<<(int const& shifts);
		BI_INT_CLASS operator<<(byte const& shifts);
		BI_INT_CLASS operator<<(uint32_t const& shifts);
		BI_INT_CLASS operator>>(byte const& shifts);

		/**
		* Subtract another unsigned big int from this one. Assumes that this int is actually bigger!
		* (i.e. does not produce a signed result)
		*/
		BI_INT_CLASS operator-(BI_INT_CLASS const& other);

		bool operator==(BI_INT_CLASS const& other);
		/**
		* Tests equality -- ALWAYS returns false if other < 0
		*/
		bool operator==(int const& other);

		/**
		*
		*/
		bool operator>=(BI_INT_CLASS const& other);

		/**
		* Multiplies this int by another and returns the result (or overflows)
		*/
		BI_INT_CLASS multiply(BI_INT_CLASS n);

		/**
		* Squares this integer and returns the result
		*/
		BI_INT_CLASS square();

		/**
		* Calculates remainder of dividing this by d, and returns result
		*/
		BI_INT_CLASS modulo(BI_INT_CLASS divisor);
		/**
		* calculates modulo of this by divisor, ignoring any sign of divisor
		*/
		BI_INT_CLASS modulo(int divisor);

		/**
		* Returns true if this number is even
		*/
		bool is_even();

		/**
		* Prints this integer as a hex string onto cout, without any newline
		*/
		void print();

		/**
		* Prints this integer as a hex string
		*/
		void println();

		/**
		* Returns the little endian byte array that stores this int internally. Size is BI_BIG_INT_SIZE!
		*/
		byte* get_byte_array();

		/**
		* Returns the first 64 bits of this number as a long (so may lose information if larger)
		*/
		unsigned long long as_long();
	};
}