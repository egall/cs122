#!/usr/bin/python
import hashlib


# Add x number of spaces to message where x = spaces and return 
def edit_message(message, spaces):
    regenerated_message = message
    for i in range (0, spaces+1):
        imod = i % 33
        regenerated_message = regenerated_message + chr(imod)
#    regen_hash = call_sha(regenerated_message)
#    short_regen_hash = regen_hash[-DIGITS_PLACE:]
    return regenerated_message



# Calls sha1 and returns the hash
def call_sha(message):
    sha = hashlib.sha1()
    sha.update(message)
    return sha.hexdigest()
    


# used to test output of sha_collision.py
if __name__ == "__main__":
    clean_message = "I, Erik Steggall, would like to buy the house on 2935 Pleasure Point Drive from Rachel Ramirez, we have agreed on the price of $2,200,000. The sale will be finalized on January 8th, 2015. -Signed Erik Q. Steggall"
    dirty_message = "I, Erik Steggall, would like to buy the house on 2935 Pleasure Point Drive from Rachel Ramirez, we have agreed on the price of $1,800,000. The sale will be finalized on January 8th, 2015. -Signed Erik Q. Steggall"
    ints_received = 0
    clean_spaces = 0
    dirty_spaces = 0
    # get number of spaces for clean message, then for dirty
    while(1):
        if(ints_received >= 2):
            break;
        else:
            if(ints_received == 0):
                clean_spaces = int(raw_input("Input number of clean spaces: "))
                print "clean_spaces = ", clean_spaces
            elif(ints_received == 1):
                dirty_spaces = int(raw_input("Input number of dirty spaces: "))
                print "dirty_spaces = ", dirty_spaces
            else:
                print "shouldn't be here\n"
        ints_received += 1
    modified_clean = edit_message(clean_message, clean_spaces)
    modified_dirty = edit_message(dirty_message, dirty_spaces)
    print call_sha(modified_clean)
    print call_sha(modified_dirty)

        

   
