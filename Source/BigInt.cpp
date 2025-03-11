#include "BigInt.h"
#include <iostream>
#include <stdlib.h>

namespace bigint {

	BI_INT_CLASS::BI_INT_CLASS() {
		zero();
	}

	BI_INT_CLASS::BI_INT_CLASS(byte byte) {
		zero();
		bytes[0] = byte;
	}

	BI_INT_CLASS::BI_INT_CLASS(const BI_INT_CLASS& other) {
		for (int i = 0; i < BI_INT_BYTE_SIZE; i++) {
			// iterate all bytes and copy 1:1
			this->bytes[i] = other.bytes[i];
		}
	}

	BI_INT_CLASS& BI_INT_CLASS::operator=(const BI_INT_CLASS& other) {
		for (int i = 0; i < BI_INT_BYTE_SIZE; i++) {
			// iterate all bytes and copy 1:1
			this->bytes[i] = other.bytes[i];
		}
		return (*this);
	}

	BI_INT_CLASS::BI_INT_CLASS(int n) {
		zero();
		// assume the int is meant to be unsigned
		n = abs(n);
		// write each byte of the int (little endian)
		for (int i = 0; i < sizeof(int); i++) {
			bytes[i] = (byte)((n >> (i * 8)) & 255);
		}
	}

	BI_INT_CLASS::BI_INT_CLASS(unsigned long long n) {
		zero();
		// write each byte of the int (little endian)
		for (int i = 0; i < sizeof(unsigned long long); i++) {
			bytes[i] = (byte)((n >> (i * 8)) & 255);
		}
	}

	BI_INT_CLASS::BI_INT_CLASS(byte bytes[BI_INT_BYTE_SIZE]) {
		for (int i = 0; i < BI_INT_BYTE_SIZE; i++) this->bytes[i] = bytes[i];
	}

	BI_INT_CLASS::BI_INT_CLASS(byte byte, unsigned int n) {
		for (unsigned int i = 0; i < n; i++) {
			bytes[i] = byte;
		}
		// write 0 to remaining bytes
		for (int i = n; i < BI_INT_BYTE_SIZE; i++) {
			bytes[i] = 0;
		}
	}

	void BI_INT_CLASS::zero() {
		for (int i = 0; i < BI_INT_BYTE_SIZE; i++) this->bytes[i] = 0;
	}

	void BI_INT_CLASS::add_byte_from(int i, byte b) {
		byte carry = b;
		for (; i < BI_INT_BYTE_SIZE; i++) {
			// loop through the number increasing significance
			// add byte each time and track carry
			carry = add_bytes(bytes + i, carry);
		}
	}

	BI_INT_CLASS BI_INT_CLASS::multiply(BI_INT_CLASS n) {
		
		BI_INT_CLASS result;
		for (int i = 0; i < BI_INT_BYTE_SIZE; i++) {
			// loop through the number n as base 256
			byte carry = 0;

			if (n.bytes[i] != 0) { // don't bother multiplying by 0 // TODO confirm this makes sense?

				for (int j = 0; j < BI_INT_BYTE_SIZE; j++) {
					// loop through this number as base 256
					// multiply the two bytes
					uint16_t mult = ((uint16_t)(n.bytes[i])) * ((uint16_t)(this->bytes[j]));
					byte low_order = (byte)(mult & 255);
					byte high_order = (byte)((mult >> 8) & 255);

					carry = add_bytes(&low_order, carry);
					result.add_byte_from(i+j, low_order);

					carry += high_order;
				}
			}
		}
		
		return result;
	}

	BI_INT_CLASS BI_INT_CLASS::operator<<(int const& shifts) {
		// do not support attempts to shift so large
		return (*this) << ((byte)(shifts & 255));
	}
	BI_INT_CLASS BI_INT_CLASS::operator<<(uint32_t const& shifts) {
		// do not support attempts to shift so large
		return (*this) << ((byte)(shifts & 255));
	}

	BI_INT_CLASS BI_INT_CLASS::operator>>(byte const& shifts) {
		// do not support shifts greater than 32 bits at a time
		// (the ints will cry) // TODO mess
		int shift_n = shifts;

		// exploit the base functionality of ints so we don't have to think
		const int ints_count = BI_INT_BYTE_SIZE / 4;
		uint32_t ints[ints_count];
		// convert bigint bytes into array of ints
		for (int i = 0; i < BI_INT_BYTE_SIZE; i += 4) {
			ints[i / 4] = ((uint32_t)(bytes[i]))
				| (((uint32_t)(bytes[i + 1])) << 8)
				| (((uint32_t)(bytes[i + 2])) << 16)
				| (((uint32_t)(bytes[i + 3])) << 24);
		}


		// for the rightmost (least significant) int, just logical shift and cut off the bottom
		ints[0] >>= shift_n;

		// shift right, starting from most significant bytes(int32)
		for (int i = 1; i < BI_INT_BYTE_SIZE / 4; i++) {
			// perform the bit shift on the value and take the top cut-off part
			uint64_t shifted = ((uint64_t)ints[i] << 32) >> shift_n;
			ints[i] = (uint32_t)(shifted >> 32 & UINT32_MAX);
			// put the cut-off part onto the empty bits of the previous larger int
			ints[i - 1] = ints[i - 1] | ((uint32_t)(shifted & UINT32_MAX));
		}

		// load result into big int
		BI_INT_CLASS result;
		for (int i = 0; i < BI_INT_BYTE_SIZE / 4; i++) {
			int k = i * 4;
			result.bytes[k] = (byte)(ints[i] & 255);
			result.bytes[k + 1] = (byte)((ints[i] >> 8) & 255);
			result.bytes[k + 2] = (byte)((ints[i] >> 16) & 255);
			result.bytes[k + 3] = (byte)((ints[i] >> 24) & 255);
		}
		// return it, bitshifted
		return result;
	}

	BI_INT_CLASS BI_INT_CLASS::operator<<(byte const& shifts) {
		// do not support shifts greater than 32 bits at a time
		// (the ints will cry)
		int shift_n = shifts;

		// exploit the base functionality of ints so we don't have to think
		const int ints_count = BI_INT_BYTE_SIZE / 4;
		uint32_t ints[ints_count];
		// convert bigint bytes into array of ints
		for (int i = 0; i < BI_INT_BYTE_SIZE; i += 4) {
			ints[i / 4] = ((uint32_t)(bytes[i])) 
				| (((uint32_t)(bytes[i+1])) << 8) 
				| (((uint32_t)(bytes[i+2])) << 16) 
				| (((uint32_t)(bytes[i+3])) << 24);
		}
		

		// for the leftmost (most significant) int, just logical shift and cut off the top
		ints[(BI_INT_BYTE_SIZE / 4) - 1] <<= shift_n;

		// shift left, starting from most significant bytes(int32)
		for (int i = (BI_INT_BYTE_SIZE / 4) - 2; i >= 0; i--) {
			// perform the bit shift on the value and take the top cut-off part
			uint64_t shifted = ((uint64_t) ints[i]) << shift_n;
			ints[i] = (uint32_t) (shifted & UINT32_MAX);
			// put the cut-off part onto the empty bits of the previous larger int
			ints[i + 1] = ints[i + 1] | ((uint32_t) ((shifted >> 32) & UINT32_MAX));
		}

		// load result into big int
		BI_INT_CLASS result;
		for (int i = 0; i < BI_INT_BYTE_SIZE / 4; i++) {
			int k = i * 4;
			result.bytes[k] = (byte) (ints[i] & 255);
			result.bytes[k + 1] = (byte)((ints[i] >> 8) & 255);
			result.bytes[k + 2] = (byte)((ints[i] >> 16) & 255);
			result.bytes[k + 3] = (byte)((ints[i] >> 24) & 255);
		}
		// return it, bitshifted
		return result;
	}

	BI_INT_CLASS BI_INT_CLASS::square() {
		// make a copy and multiply them
		BI_INT_CLASS value = *this;
		return multiply(value);
	}

	byte BI_INT_CLASS::add_bytes(byte* x, byte y) {
		uint16_t result = ((uint16_t) (*x)) + ((uint16_t)y);
		*x = (byte)(result & 255);
		return (byte)((result >> 8) & 255); // bitwise & is technically redundant because of shift -- readability
	}

	bool BI_INT_CLASS::operator>=(BI_INT_CLASS const& other) {
		// loop from the most significant byte, looking for a byte that is smaller
		for (int i = BI_INT_BYTE_SIZE - 1; i >= 0; i--) {
			if (this->bytes[i] < other.bytes[i]) {
				// if we've found a smaller one first, the other number is bigger
				return false;
			}
			else if (this->bytes[i] > other.bytes[i]) {
				// if we find a bigger one first, this number is bigger
				return true;
			}
		}

		// they are equal
		return true;
	}

	BI_INT_CLASS BI_INT_CLASS::operator-(BI_INT_CLASS const& other) {
		BI_INT_CLASS result;
		// copy of this so we don't modify it when doing borrows
		BI_INT_CLASS copy(this->bytes);
		
		// go through byte by byte starting with least significant
		for (int i = 0; i < BI_INT_BYTE_SIZE; i++) {
			// make an int that can store any borrows this will acquire
			uint16_t byte_and_carry = copy.bytes[i];

			if (byte_and_carry < other.bytes[i]) {
				// we need to borrow from a more significant byte
				// so look for one
				int j = i;
				while (j < BI_INT_BYTE_SIZE - 1) {
					j++;
					if (copy.bytes[j] != 0) break;
				}
				//while ((j < INT_BYTE_SIZE - 1) && (this->bytes[j++] == 0));
				// once we find a more significant byte that is not zero, borrow from it
				copy.bytes[j]--;
				// then drop borrows for any bytes in between
				for (j--; j > i; j--) {
					// since we're borrowing immediately (this byte was 0), each in-between byte becomes 255
					copy.bytes[j] = 0xff;
				}
				// fill in the final borrow
				byte_and_carry += 0x100;
				// in order to subtract these bytes now
			} // (otherwise we can subtract this byte without borrowing anyway)

			result.bytes[i] = byte_and_carry - other.bytes[i];
		}

		return result;
	}

	bool BI_INT_CLASS::operator==(BI_INT_CLASS const& other) {
		for (int i = 0; i < BI_INT_BYTE_SIZE; i++) {
			if (other.bytes[i] != this->bytes[i]) return false;
		}
		return true;
	}
	bool BI_INT_CLASS::operator==(int const& other) {
		if (other < 0) return false;
		BI_INT_CLASS to_big_int(other);
		return (*this) == to_big_int;
	}

	BI_INT_CLASS BI_INT_CLASS::modulo(BI_INT_CLASS divisor) {
		// implementation: see https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
		BI_INT_CLASS remainder;
		// loop starting from most significant bit
		for (int i = BI_INT_SIZE - 1; i >= 0; i--) {
			// shift the remainder along one bit
			remainder = remainder << 1;
			// select the ith bit of the numerator (this) -- in the '1' place value
			// i.e. start in most significant byte
			byte ith = this->bytes[i / 8];
			ith = ith >> (i % 8); // TODO does this work?
			ith = ith & (byte) 1;
			// smaller bit of remainder becomes numerator current bit
			remainder.bytes[0] = remainder.bytes[0] | (ith);
			// if current remainder is bigger than the divisor
			if (remainder >= divisor) {
				// subtract divisor from remainder
				remainder = remainder - divisor;
			}

		}
		return remainder;
	}

	BI_INT_CLASS BI_INT_CLASS::modulo(int divisor) {
		return this->modulo(BI_INT_CLASS(divisor));
	}

	bool BI_INT_CLASS::is_even() {
		return bytes[0] & 1;
	}

	void BI_INT_CLASS::print() {
		// loop backwards since bytes are organised little endian
		for (int i = BI_INT_BYTE_SIZE - 1; i >= 0; i--) {
			// output each byte as two hex digits
			printf("%02x", (int) bytes[i]);
		}
	}

	void BI_INT_CLASS::println() {
		this->print();
		printf("\n");
	}

	byte* BI_INT_CLASS::get_byte_array() {
		return bytes;
	}

	unsigned long long BI_INT_CLASS::as_long() {
		return this->as_long_from(0);
	}


	unsigned long long BI_INT_CLASS::as_long_from(unsigned int n) {
		unsigned long long lower = 0;
		for (int i = n; i < n + 8; i++) {
			lower |= (((unsigned long long) (bytes[i])) << ((i-n) * 8));
		}
		return lower;
	}
}