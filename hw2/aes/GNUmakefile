#
# Makefile for simple AES encrypt / decrypt program.  Has options for both software-only
# AES as well as AES intrinsic instructions in newer x86 processors.
#
# NOTE: has GNU make specific stuff
#

# Use the flags below if you don't have hardware support for AES
#AES_SRC = rijndael.c
#PROC_FLAGS =
# Use the flags below if you *do* have hardware support for AES.  It's considerably faster!
AES_SRC = aes_x86.c
PROC_FLAGS = -msse4.2 -maes

#----------------------------------------------------------------------
#----------------------------------------------------------------------
SRCS = aes.c $(AES_SRC)
INCS = aes.h
TARGET = aes
CFILES = aes.c aes_x86.c rijndael.c $(INCS)
OTHERS = testfile GNUmakefile
DIR = $(notdir $(PWD))
TAR = tar
TARFILE = aes.tgz
OBJS = $(addsuffix .o, $(basename $(SRCS)))
CFLAGS = -O3 $(PROC_FLAGS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

rijndael.o: aes.h
aes_x86.o: aes.h
aes.o: aes.h

tarball:
	cd .. ; $(TAR) cvf - $(addprefix $(DIR)/, $(CFILES) $(OTHERS)) | gzip - > $(TARFILE)

test: $(TARGET)
	@echo "NOTE: a difference in the last line is probably OK---It's"
	@echo "      due to padding the last block encrypted."
	@echo
	./aes -e -1 abcd < testfile | ./aes -d -1 abcd | diff - testfile
	./aes -e -1 123456 < testfile | ./aes -d -1 123456 | diff - testfile
	./aes -e -1 abcd71234 < testfile | ./aes -d -1 abcd71234 | diff - testfile
	./aes -e -2 abcd < testfile | ./aes -d -2 abcd | diff - testfile

clean:
	$(RM) *~ *.o

spotless: clean
	$(RM) $(TARGET)
