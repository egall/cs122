#!/usr/bin/python

import os
import hashlib


def check_hashes_final(full_regenerated_tup):
    regenerated_message_clean = full_regenerated_tup[0]
    regenerated_message_dirty = full_regenerated_dirty[0]
    clean_regen_hash = call_sha(regenerated_message_clean)
    dirty_regen_hash = call_sha(regenerated_message_dirty)
    print "clean hash = ", clean_regen_hash
    print "dirty hash = ", dirty_regen_hash


def regenerate_message(message, number_of_spaces):
    regenerated_message = message
    for i in range (0, number_of_spaces+1):
        regenerated_message = regenerated_message + " "
    regen_hash = call_sha(regenerated_message)
    short_regen_hash = regen_hash[-2:]
    print "regen = ", short_regen_hash
    return regenerated_message

# Both clean and dirty tups have the value of (hash, # of spaces)
# Matched tups contains (clean_tup, dirty_tup)
def regenerate_messages(matched_tups, clean_message, dirty_message):
    clean_tup = matched_tups[0]
    dirty_tup = matched_tups[1]
    
    full_regenerated_clean = regenerate_message(clean_message, clean_tup[1])
    full_regenerated_dirty = regenerate_message(dirty_message, dirty_tup[1])

    full_regenerated_tup = (full_regenerated_clean, full_regenerated_dirty)
    return full_regenerated_tup
   

def generate_messages(clean_message):
    new_message = clean_message
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
    return hash_dict

def generate_dirty_collision(dirty_message, clean_dict):
    print dirty_message
    print clean_dict
    new_dirty_message = dirty_message
    for i in range(0, 128):
        new_dirty_message = new_dirty_message + " "
        dhash_val = call_sha(new_dirty_message)
        short_dhash = dhash_val[-2:]
        if short_dhash in clean_dict.keys():
            clean_tup = (short_dhash, clean_dict[short_dhash])
            dirty_tup = (short_dhash, i)
            matched_tups = (clean_tup, dirty_tup)
            return matched_tups
            print clean_tup
            print dirty_tup
    

def call_sha(message):
    sha = hashlib.sha1()
    sha.update(message)
    return sha.hexdigest()
    

if __name__ == "__main__":
    clean_message = "I, Erik Steggall, would like to buy the house on 2935 Pleasure Point Drive from Rachel Ramirez, we have agreed on the price of $2,200,000. The sale will be finalized on January 8th, 2015. -Signed Erik Q. Steggall"
    dirty_message = "I, Erik Steggall, would like to buy the house on 2935 Pleasure Point Drive from Rachel Ramirez, we have agreed on the price of $1,800,000. The sale will be finalized on January 8th, 2015. -Signed Erik Q. Steggall"
    clean_hash_dict = {}
    clean_dict = generate_messages(clean_message)
    matched_tups = generate_dirty_collision(dirty_message, clean_dict)
    full_regenerated_tup = regenerate_messages(matched_tups, clean_message, dirty_message)
    print full_regenerated_tup
  
    
    
