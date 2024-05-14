# Generate random lists of integers and write them to a file
import random

def generateList(n):
    return [random.randint(0, 100) for _ in range(n)]

def writeListToFile(lst, filename):
    with open(filename, 'w') as f:
        for item in lst:
            f.write(f"{item}\n")

if __name__ == "__main__":
    exponents = [3, 4, 5, 6,7]
    for exp in exponents:
        n = 10 ** exp
        lst = generateList(n)
        writeListToFile(lst, f"./results/randomList-{n}.txt")
        print(f"Generated list of {n} elements and wrote it to randomList{n}.txt")
