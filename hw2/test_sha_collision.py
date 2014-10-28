#!/usr/bin/python
import hashlib

message_second_half = "PM, 2015. -Signed Erik Q. Steggall"


# Regenerate message
def edit_message(message_first_half, i):
    full_message = message_first_half + str(i) + message_second_half
    return full_message

# Calls sha1 and returns the hash
def call_sha(message):
    sha = hashlib.sha1()
    sha.update(message)
    return sha.hexdigest()

# used to test output of sha_collision.py
if __name__ == "__main__":
    clean_message = "I, Erik Steggall, would like to buy the house on 2935 Pleasure Point Drive from Rachel Ramirez, we have agreed on the price of $2,200,000. The sale will be finalized on January 8th at 14:00."
    dirty_message = "I, Erik Steggall, would like to buy the house on 2935 Pleasure Point Drive from Rachel Ramirez, we have agreed on the price of $1,800,000. The sale will be finalized on January 8th at 14:00."
    ints_received = 0
    clean_time = 0
    dirty_time = 0
    # get number of spaces for clean message, then for dirty
    while(1):
        if(ints_received >= 2):
            break;
        else:
            if(ints_received == 0):
                clean_time = int(raw_input("Input time extension: "))
                print "clean_time = ", clean_time
            elif(ints_received == 1):
                dirty_time = int(raw_input("Input time extension: "))
                print "dirty_time = ", dirty_time
            else:
                print "shouldn't be here\n"
        ints_received += 1
    modified_clean = edit_message(clean_message, clean_time)
    modified_dirty = edit_message(dirty_message, dirty_time)
    print call_sha(modified_clean)
    print call_sha(modified_dirty)

        

   
