primes = [2]

def generate_probable_primes(my_range):
    probable_primes = []
    for i in range(4, my_range):
        print(f"Running probable_generation {i} / {my_range}")
        if is_probable_prime(i):
            probable_primes.append(i)
    return probable_primes
  
def check_if_probable_primes_are_primes(probable_primes):
    count = 0
    size = len(probable_primes)
    for i in probable_primes:
        count += 1
        print(f"Checking if probable is prime {count}/{size}")
        if is_prime(i):
            primes.append(i)
          
def is_prime(num):
    for i in primes:
        if not (num % i):
            return False
    return True
    
def is_probable_prime(num):
    import random
    if num <= 3:
        raise Exception("Bad num")
    for i in range(1, 2):
        # test for composite
        a = random.randint(2, num - 2)
        if pow(a, num - 1) % num != 1:
            return False
    return True

def main():
    import sys
    range = pow(10,6) + 1
    # range = 15
    probable_primes = generate_probable_primes(range)
    check_if_probable_primes_are_primes(probable_primes)
    orig_stdout = sys.stdout
    with open('primes.txt', 'w') as f:
        sys.stdout = f
        print(primes)
        sys.stdout = orig_stdout
        f.close()

if __name__ == "__main__":
    main()