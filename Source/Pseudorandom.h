#pragma once
#include "BigInt.h"
#include "Dev.h"

using namespace bigint;

namespace cry {
/**
* Generates 512-bit psuedorandom numbers using the blum-blum-shub algorithm
*/
class Pseudorandom {
private:
	/** 
	* The divisor is the product of two large primes p and q for which
	*  p % 4 == 3 and q % 4 == 3; that are safe primes (i.e. (p-1) / 2 is prime, same for q);
	*  with a small greatest common divisor between (p-3)/2 and (q-3)/2;
	*/
	int256 divisor;
	/**
	* The most recently generated value -- or at time of construction, the seed.
	* The seed should be prime.
	*/
	int256 current;

	/**
	* Returns true if given n is prime. ASSUMING THAT n IS NOT EVEN!
	*/
	bool is_prime(longest n);

	/**
	* Returns the next prime number from after (not including) start
	*/
	longest find_next_prime(longest start);
	/**
	* Returns the next prime number from after (not including start) for which:
	*  n % 4 == 3
	*  (n-1) / 2 is prime (safe)
	*/
	longest find_next_factor_prime(longest start);

public:
	/**
	* Where seed1 is any value significantly greater than seed 2
	*/
	Pseudorandom(longest seed1, longest seed2);

	Pseudorandom();

	/**
	* Returns the next pseudorandom. must have constructed with seed to return sensical random result
	*/
	int256 next();
	
};

}