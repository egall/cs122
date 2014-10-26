#!/usr/bin/python

import operator

def get_divisors(num):
    div_list = []
    for i in range(2, num):
        if num % i == 0:
            div_list.append(i)
    return div_list

def sort_div_freq_list(div_freq_list):
    div_dict = {}
    for div_list in div_freq_list:
        #print div_list
        for number in div_list:
            if number in div_dict:
                div_dict[number] += 1
            else:
                div_dict[number] = 1
    return div_dict

if __name__ == "__main__":
    letter_dict = {}
    trigram_dict = {}
    filename = "vigenere-encrypted.txt"
    trigram = ""
    match_trigram = ""
    more_bytes = 3
    num_bytes = 0
    distance = 0
    offset = 0
    div_freq_list = []
    with open(filename, "rb") as f:
        for line in f:
            for i in range(0, len(line)):
                letter = line[i]
                # stop getting trigrams at len - 2
                # (the last char is a newline)
                if(i < len(line)-3):
                    trigram = line[i] + line[i+1] + line[i+2]
                more_bytes = 3
                for j in range(i+1, len(line)-3):
                    match_trigram = line[j] + line[j+1] + line[j+2]
                    if (match_trigram == trigram):
                        #print "matched trigram = ", match_trigram
                        while(line[i:i+more_bytes] == line[j:j+more_bytes]):
                            #print "more match = ", line[i:i+more_bytes]
                            more_bytes += 1
                        offset = j + more_bytes
                        distance = j - i
                        div_freq_list.append(get_divisors(distance))
                        #print "off = ", offset
                        #print "dist = ", distance
    div_dict = sort_div_freq_list(div_freq_list)
    sorted_div_dict = sorted(div_dict.items(), key=operator.itemgetter(1))
    print sorted_div_dict
    #print "div freq list = ", div_freq_list                  
    #sorted_letters = sorted(letter_dict.items(), key=operator.itemgetter(1))
    #sorted_trigram = sorted(trigram_dict.items(), key=operator.itemgetter(1))
    #print "Sorted letters = ", sorted_letters
    #print "trigram = ", sorted_trigram
    f.close()
