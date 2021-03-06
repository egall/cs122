#!/usr/bin/python

import os
import hashlib

#DIGITS_PLACE = 8
#DIGITS_PLACE = 9
DIGITS_PLACE = 10
#DIGITS_PLACE = 12
#NUMBER_OF_ITERATIONS = 16777216
#NUMBER_OF_ITERATIONS = 65536
#NUMBER_OF_ITERATIONS = 131072
#NUMBER_OF_ITERATIONS = 262144
NUMBER_OF_ITERATIONS = 524288
#NUMBER_OF_ITERATIONS = 33554432


# Test to see if the hashes match
# Returns: None, it simply prints the two hashes for manual verification
def check_hashes_final(full_regenerated_tup):
    regenerated_message_clean = full_regenerated_tup[0]
    regenerated_message_dirty = full_regenerated_tup[1]
    clean_regen_hash = call_sha(regenerated_message_clean)
    dirty_regen_hash = call_sha(regenerated_message_dirty)
    print "clean hash = ", clean_regen_hash
    print "dirty hash = ", dirty_regen_hash


# This function regenerates the messages that were found have a collision
# Returns: Message + number of spaces specified
def regenerate_message(message, number_of_spaces):
    regenerated_message = message
    for i in range (0, number_of_spaces+1):
        imod = i % 33
        regenerated_message = regenerated_message + chr(imod)
    regen_hash = call_sha(regenerated_message)
    short_regen_hash = regen_hash[-DIGITS_PLACE:]
    return regenerated_message

# Both clean and dirty tups have the value of (hash, # of spaces)
# Matched tups contains (clean_tup, dirty_tup)
# Returns: A tuple with both of the regenerated messages
def regenerate_messages(matched_tups, clean_message, dirty_message):
    clean_tup = matched_tups[0]
    dirty_tup = matched_tups[1]
    
    full_regenerated_clean = regenerate_message(clean_message, clean_tup[1])
    full_regenerated_dirty = regenerate_message(dirty_message, dirty_tup[1])

    full_regenerated_tup = (full_regenerated_clean, full_regenerated_dirty)
    return full_regenerated_tup
   

# This function runs through a loop adding spaces to clean_message
# and hashing it. If the hash has not been seen before it's added
# to a hash table, otherwise it continues onto the next one.
# Returns: 
def generate_messages(clean_message):
    new_message = clean_message
    hash_dict = {}
    num_col = 0
    #sha = hashlib.sha1()
    for i in range(0, NUMBER_OF_ITERATIONS):
        imod = i % 33
        new_message = new_message + chr(imod)
        hash_val = call_sha(new_message)
        #sha.update(new_message)
        #hash_val = sha.hexdigest()
        # hash_tup hold the hash and the number of spaces it holds 
        short_hash_val = hash_val[-DIGITS_PLACE:]
        if short_hash_val in hash_dict:
            num_col += 1
        else:
            hash_dict[short_hash_val] = i
        hash_tup = (short_hash_val, i)
    print "Number of collisions =", num_col
    return hash_dict

# This function runs through a loop adding spaces to dirty_message
# and hashing the new message. It then checks to see if the hash
# exists in the clean_message's hash dictionary. If so, it stops
# and returns
# Returns: Tuple that contains two tuples:
#          First tuple - clean hash, number of spaces added in order to genereate
#          Second tuple - dirty hash, number of spaces added in order to genereate
def generate_dirty_collision(dirty_message, clean_dict):
    print dirty_message
    new_dirty_message = dirty_message
    #sha = hashlib.sha1()
    for i in range(0, NUMBER_OF_ITERATIONS*2):
        imod = i % 33
        new_dirty_message = new_dirty_message + chr(imod)
        dhash_val = call_sha(new_dirty_message)
        #sha.update(new_dirty_message)
        #dhash_val = sha.hexdigest()
        short_dhash = dhash_val[-DIGITS_PLACE:]
        if short_dhash in clean_dict:
            clean_tup = (short_dhash, clean_dict[short_dhash])
            dirty_tup = (short_dhash, i)
            matched_tups = (clean_tup, dirty_tup)
            return matched_tups
    

# Simply calls sha on the given message and returns
def call_sha(message):
    sha = hashlib.sha1()
    sha.update(message)
    return sha.hexdigest()
    

# This program calls two functions, generate_message() and generate_dirty_collision()
# It then verifies that these hashes do indeed cause a collison
if __name__ == "__main__":
    clean_message = "I, Erik Steggall, would like to buy the house on 2935 Pleasure Point Drive from Rachel Ramirez, we have agreed on the price of $2,200,000. The sale will be finalized on January 8th, 2015. -Signed Erik Q. Steggall"
    dirty_message = "I, Erik Steggall, would like to buy the house on 2935 Pleasure Point Drive from Rachel Ramirez, we have agreed on the price of $1,800,000. The sale will be finalized on January 8th, 2015. -Signed Erik Q. Steggall"
    clean_hash_dict = {}
    clean_dict = generate_messages(clean_message)
    matched_tups = generate_dirty_collision(dirty_message, clean_dict)
    full_regenerated_tup = regenerate_messages(matched_tups, clean_message, dirty_message)
    check_hashes_final(full_regenerated_tup)
    clean_tup = matched_tups[0]
    dirty_tup = matched_tups[1]
    number_of_clean_spaces = clean_tup[1]
    number_of_dirty_spaces = dirty_tup[1]
    print("clean message is {} \nnumber of spaces in it are {}".format(clean_message, number_of_clean_spaces))
    print("dirty message is {} \nnumber of spaces in it are {}".format(dirty_message, number_of_dirty_spaces))
  
    
    
