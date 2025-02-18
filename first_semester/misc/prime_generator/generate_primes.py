primes = [2]

def sieve_of_eratosthenes(n):
    # Step 1: Create a list of booleans, initially set to True
    is_prime = [True] * (n + 1)
    p = 2
    
    # Step 2: Set 0 and 1 to not prime
    is_prime[0] = is_prime[1] = False
    
    # Step 3: Iterate over each number up to the square root of n
    while p * p <= n:
        # If p is prime, mark all its multiples as not prime
        if is_prime[p]:
            for i in range(p * p, n + 1, p):
                is_prime[i] = False
        p += 1
    
    # Step 4: Collect all numbers that are still marked as prime
    primes = [p for p in range(n + 1) if is_prime[p]]
    return primes


def main():
    import sys
    range = pow(10,6) + 1
    primes = sieve_of_eratosthenes(range)
    orig_stdout = sys.stdout
    with open('primes.txt', 'w') as f:
        sys.stdout = f
        print(primes)
        sys.stdout = orig_stdout
        f.close()

if __name__ == "__main__":
    main()