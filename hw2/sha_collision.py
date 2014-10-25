#!/usr/bin/python

import os
import hashlib


def generate_messages(original_message):
    new_message = original_message
    hash_dict = {}
    num_col = 0
    for i in range(0, 128):
        new_message = new_message + " "
        hash_val = call_sha(new_message)
        # hash_tup hold the hash and the number of spaces it holds 
        short_hash_val = hash_val[-2:]
        if short_hash_val in hash_dict.keys():
            print "collision!\n"
            num_col += 1
        else:
            hash_dict[short_hash_val] = i
        hash_tup = (short_hash_val, i)
    print "Number of collisions =", num_col
    print hash_dict

def call_sha(message):
    sha = hashlib.sha1()
    sha.update(message)
    return sha.hexdigest()
    

if __name__ == "__main__":
    message = "The little brown fox jumped over the lazy dog"
    generate_messages(message)
