#
#  pseudorandom.py
#    For experimenting with pseudorandom number generation, specifically
#    Blum Blum Shub. Functions to find suitable prime numbers and test
#    how long it takes for the algorithm to cycle.
#  Note: code is awful test code, not for shipping in any capacity.
#

def is_prime(n):
    if n < 2: return False
    for i in range(2, int((n/2) + 1)):
        if n % i == 0:
            return False
    return True

# returns next prime number from and including n
def next_prime_from(n):
    # don't bother checking if an even number is prime
    if n % 2 == 0: n += 1
    
    while not is_prime(n):
        # skip evens
        n += 2
    return n

# returns tuple of Sophie Germain prime and corresponding safe prime
def next_safe_prime_from(n):
    # safe primes are primes where (n - 1)/2 is also prime
    while True:
        n = next_prime_from(n)
        if is_prime(2*n + 1):
            return (n, 2*n + 1)
        else:
            n += 2 # add two to skip over the next even


# returns tuple of Sophie Germain prime and corresponding safe prime, assuming
# prime is congruent to 3 (mod 4)
def next_safe_congruent_prime_from(n):
    n = (n, n)
    while True:
        n = next_safe_prime_from(n[0])
        if n[0] % 4 == 3:
            return n
        else: n = (n[0] + 2, 0) # skip evens

def bbs(x):
    return (x**2) % (p*q)

def dummy(x):
    # a function that will cycle
    return (x+1) % 12

# Blum Blum Shub algorithm
# x(n+1) = x(n)^2 mod (pq)
# [theory] should always cycle before (pq) iterations

def find_cycles():
    # tracking cycles by checking for biggest output
    biggest = 0
    cycle = 0
    cycle_count = 0
    n = seed
    # try 10 billion generations
    for i in range(10000000000):
        n = bbs(n)
        if n > biggest:
            biggest = n
        elif n == biggest:
            if cycle == 0:
                cycle = i # only track first cycle
            cycle_count += 1
        if i%10000==0:
            # print every 10000ish
            print(str(i) + (" " * (32-len(str(i)+str(n)))), n)
        if cycle_count == 2:
            # stop checking once a few have been found
            break

    if cycle != 0:
        # the index of the cycle indicates that the algorithm loops around
        # (cycle/2) index
        print("First cycle detected at index",cycle)
        print("Found",cycle_count,"cycles")
    else:
        print("No cycle detected in this range")

    print("Seed:", seed)
    print("p:", p)
    print("q:", q)

def until_cycle():
    i = 1
    biggest = 0
    n = seed
    while i < 150:
        n = bbs(n)
        # print every hundred millionth term
        if (i % 1 == 0):
            print(str(i)+"th term is",n)
        if n > biggest:
            biggest = n
        elif n == biggest:
            print("Cycle found at", i)
            break
        i += 1

def main():
    seed = next_prime_from(89775676)
    p = next_safe_congruent_prime_from(10**9 + 10**7)[1]
    q = next_safe_congruent_prime_from(11**8)[1]

    print("Seed:", seed)
    print("p:", p)
    print("q:", q)
    print("pq:", (p*q))

    # check for co prime
    print("seed%p:",seed%p,"seed%q:",seed%q)

    # cycling time
    until_cycle()

if __name__=='__main__':
    main()
    
