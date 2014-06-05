import subprocess
import resource
import random
import math

def time_algorithm(data, repeats):
    with open('test.txt', 'w') as file:
        file.write(data)
    sum_of_repeats = 0
    for repeat in range(repeats):
        start = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime
        p = subprocess.Popen(['./tournament-fixing-algorithm.o', 'test.txt'], stdout=subprocess.PIPE)
        #print(p.communicate())
        sum_of_repeats += resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime - start
    return sum_of_repeats / repeats


def generate_case(size, cases, randoms):
    case = 'players {};winner 0;'.format(size)
    numbers = []
    for i in range(1, size):
        numbers.append(i)

    for i in range(cases):
        decisions = []
        posibilities = numbers[:]
        for i in range(random.randint(int(math.log2(size)), size - 1)):
            r = random.randint(0, len(posibilities) - 1)
            decisions.append('0 > {};'.format(posibilities[r]))
            del posibilities[r]
        for j in range(randoms):
            first = random.randint(1, size - 1)
            second = random.randint(1, size - 1)
            if(second != first):
                decisions.append('{} > {};'.format(first, second))
        yield case + ''.join(decisions)

size = 4
for i in range(3):
    print('grupa {}'.format(size))
    for case in generate_case(size, 10, size//2):
        print(time_algorithm(case, 5))
    size *= 2