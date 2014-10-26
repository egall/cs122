#!/usr/bin/python

import operator

def calc_letter_freq():
    
    print "hello world"

if __name__ == "__main__":
    letter_dict = {}
    bigram_dict = {}
    trigram_dict = {}
    quadgram_dict = {}
    filename = "mono-encrypted.txt"
    trigram = ""
    bigram = ""
    with open(filename, "rb") as f:
        for line in f:
            for i in range(0, len(line)):
                letter = line[i]
                # stop getting bigrams at len - 2
                # (the last char is a newline)
                if(i < len(line)-2):
                    bigram = line[i] + line[i+1]
                    # Store bigrams in dictionary 
                    if bigram in bigram_dict.keys():
                        bigram_dict[bigram] += 1
                    else: 
                        bigram_dict[bigram] = 1
                    # stop getting bigrams at len - 3
                    if(i < len(line)-3):
                        trigram = line[i] + line[i+1] + line[i+2]
                        # Store trigrams in dictionary 
                        if trigram in trigram_dict.keys():
                            trigram_dict[trigram] += 1
                        else: 
                            trigram_dict[trigram] = 1
                        if(i < len(line)-4):
                            quadgram = line[i] + line[i+1] + line[i+2] + line[i+3]
                            # Store trigrams in dictionary 
                            if quadgram in quadgram_dict.keys():
                                quadgram_dict[quadgram] += 1
                            else: 
                                quadgram_dict[quadgram] = 1
 
                # Store letters in dictionary by freq
                if letter in letter_dict.keys():
                    letter_dict[letter] += 1
                else: 
                    letter_dict[letter] = 1
                 
    sorted_letters = sorted(letter_dict.items(), key=operator.itemgetter(1))
    sorted_bigram = sorted(bigram_dict.items(), key=operator.itemgetter(1))
    sorted_trigram = sorted(trigram_dict.items(), key=operator.itemgetter(1))
    sorted_quadgram = sorted(quadgram_dict.items(), key=operator.itemgetter(1))
    print "Sorted letters = ", sorted_letters
    print "bigram = ", sorted_bigram
    print "trigram = ", sorted_trigram
    print "quadgram = ", sorted_quadgram
    """
    print "letter dict = ", letter_dict
    print "bigram dict = ", bigram_dict
    print "trigram dict = ", trigram_dict
    """
    f.close()
