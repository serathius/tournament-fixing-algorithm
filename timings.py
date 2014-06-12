import subprocess
import resource
import random
import math
import signal

class Alarm(Exception):
    pass

def alarm_handler(signum, frame):
    raise Alarm

def time_algorithm(data, repeats):
    with open('test.txt', 'w') as file:
        file.write(data)
    time = 0
    for i in range(repeats):
        signal.signal(signal.SIGALRM, alarm_handler)
        signal.alarm(1)
        try:
            p = subprocess.Popen(['./tournament-fixing-algorithm.o', 'test.txt'], stdout=subprocess.PIPE)
            result = p.communicate()
            if(result[0] == b''):
                return None
            case = result[0].decode("ascii")[0]
            time += float(result[0][1:8])
            signal.alarm(0)
        except ValueError:
            return None
        except Alarm:
            return None
    return case, time / repeats

def generate_case_A(size, cases, randoms):
    case = 'players {};winner 0;'.format(size)
    numbers = []
    for i in range(1, size):
        numbers.append(i)

    decisions = set()
    posibilities = numbers[:]
    for i in range(random.randint(int(math.log2(size))+1, size - 1)):
        r = random.randint(0, len(posibilities) - 1)
        decisions.add('0 > {};'.format(posibilities[r]))
        del posibilities[r]
    for i in range(len(posibilities)):
        for j in range(len(numbers) - len(posibilities)):
            r = random.randint(0, len(posibilities) - 1)
            decisions.add('{} > {};'.format(i, posibilities[r]))
        del posibilities[r]    
    for j in range(randoms):
        first = random.randint(1, size - 2)
        second = random.randint(first, size - 1)
        if(second != first):
            decisions.add('{} > {};'.format(first, second))
    return case + ''.join(decisions)

def generate_case_B(size, cases, randoms):
    case = 'players {};winner 0;'.format(size)
    numbers = []
    for i in range(1, size):
        numbers.append(i)

    decisions = set()
    posibilities = numbers[:]
    for i in range(random.randint(size/2, size - 1)):
        r = random.randint(0, len(posibilities) - 1)
        decisions.add('0 > {};'.format(posibilities[r]))
        del posibilities[r]
    for j in range(randoms):
        first = random.randint(1, size - 2)
        second = random.randint(first, size - 1)
        if(second != first):
            decisions.add('{} > {};'.format(first, second))
    return case + ''.join(decisions)

def generate_case_C(size, cases, randoms):
    case = 'players {};winner 0;'.format(size)
    numbers = []
    for i in range(1, size):
        numbers.append(i)
    decisions = set()
    wins = set()
    for i in range(random.randint(int(math.log2(size))+1, size/2 - 1)):
        r = numbers[0]
        wins.add(r)
        decisions.add('0 > {};'.format(r))
        del numbers[0]
    for i in wins:
        for j in numbers:
            decisions.add('{} > {};'.format(i, j))
    
    for j in range(randoms):
        first = random.randint(1, size - 2)
        second = random.randint(first, size - 1)
        if(second != first):
            decisions.add('{} > {};'.format(first, second))
    return case + ''.join(decisions)

used_func = generate_case_C
results = {'A':{}, 'B':{}, 'C':{}}
for size in (2**x for x in range(9,11)):
    for j in range(25):
        case = used_func(size, 5, size//2)
        t = time_algorithm(case, 5)
        try_count =0
        while(t == None or try_count<1):
            case = used_func(size, 5, size//2)
            t = time_algorithm(case, 5)
            try_count+=1
        if t == None:
            continue
        case, time = t
        if size in results[case]:
            results[case][size].append(time)
        else:
            results[case][size] = [time]
print('A')
for k,v in results['A'].items():
    print(k)
    for b in v:
        print(b)
print('B')
for k,v in results['B'].items():
    print(k)
    for b in v:
        print(b)
print('C')
for k,v in results['C'].items():
    print(k)
    for b in v:
        print(b)