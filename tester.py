import random

def generate_random_string(length):
    """Generates a random DNA string of a specified length."""
    return ''.join(random.choice('ACGT') for _ in range(length))

# Define the lengths for the test strings
lengths = [5, 25, 150, 500, 1000, 2500, 5000, 10000,15000, 25000]

# Generate pairs of random strings for each specified length
test_data = {}
for length in lengths:
    str1 = generate_random_string(length)
    str2 = generate_random_string(length)
    test_data[length] = (str1, str2)

# Save the generated strings to text files
file_paths = []
for length, (str1, str2) in test_data.items():
    with open(f'X_{length}.txt', 'w') as f:
        f.write(f'{str1}')
    file_paths.append(f'X_{length}.txt')
    with open(f'Y_{length}.txt', 'w') as f:
        f.write(f'{str2}')
    file_paths.append(f'Y_{length}.txt')
file_paths