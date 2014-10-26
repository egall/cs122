#!/usr/bin/python


def get_factors():
    first = []
    second = []
    with open("primes.txt", "rb") as f:
        for line in f:
            line = line.split()
            for number in line:
                print "number = ", number
                first.append(number)
                second.append(number)

    for i in range(0, len(first)-1):
        for j in range(0, len(first)-1):
            if ( (int(first[i]) * int(second[j])) == 3953):
                print "Got it!\n"
                print "first = ", first[i]
                print "second = ", second[j]

if __name__ == "__main__":
    p = 67
    q = 59
    tot_n = (p-1)*(q-1)
    print "tot n = ", tot_n
    e = 101
    for d in range(0, tot_n):
        if ( ((e*d) % tot_n) == 1):
            print d
