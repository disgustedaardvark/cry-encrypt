#include "Pseudorandom.h"
#include <iostream>

namespace cry {

	Pseudorandom::Pseudorandom() {
		// throw in some values to produce 0, 0, 0 as recurrence
		divisor = 1;
		current = 0;
		// so its obvious that construction was wrong
	}

	Pseudorandom::Pseudorandom(longest seed1, longest seed2) {
		dev(std::cout << "Seeds are: " << seed1 << " and " << seed2 << std::endl);
		// find a valid p and q value and store their multiplication
		longest p = find_next_factor_prime(seed2);
		longest q = find_next_factor_prime(p);
		divisor = int256(p).multiply(int256(q));
		// find a prime number based on the given seed
		current = int256(find_next_prime(seed1));
		dev(std::cout << "Found divisor: ";
		divisor.println();
		std::cout << "Found seed:    ";
		current.println());
	}

	int256 Pseudorandom::next() {
		// do Blum-Blum-Shub algorithm to find x(n+1) from x(n)
		current = (current.square().modulo(divisor));
		return current;
	}

	longest Pseudorandom::find_next_factor_prime(longest start) {
		// set this up to skip over evens
		if (!(start & 1)) start--;

		while (true) {
			// increment, skipping evens
			start += 2;
			// get the next prime
			start = find_next_prime(start);
			// test if it fits the criteria
			if (start % 4 == 3 && is_prime((start >> 1))) {
				return start;
			}
			// keep looking
		}
	}

	longest Pseudorandom::find_next_prime(longest start) {

		// set this up to skip over evens
		if (!(start & 1)) start--;
		
		while (true) {
			// increment, skipping evens, and check if its prime
			if (is_prime(start += 2)) {
				return start;
			}
			// keep looking for prime
		}
	}

	bool Pseudorandom::is_prime(longest n) {
		// max factor for n would be n / 2 + 1
		longest max = (n >> 1) + 1;
		// check all factors lower
		// start from 3 because we're assuming we have already eliminated evens
		for (longest i = 3; max >= i; i++) {

			if (n % i == 0) {
				// not prime
				return false;
			}
		}
		return true;
	}
}