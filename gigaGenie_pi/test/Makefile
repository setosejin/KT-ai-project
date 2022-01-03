############################################
# This is a project standard makefile..
############################################
SHELL	= /bin/sh
CC	= gcc
AR	= ar ruvs
RANLIB	= ranlib 
LEX	= lex
YACC	= yacc
INGCC	= esqlc

###########################################
# IOTMAKERS_SDK_HOME
###########################################
IOTMAKERS_SDK_HOME = ..
IOTMAKERS_SDK_LIBNAME = libiotmakers.a

IOTMAKERS_SDK_LIB_PATH = $(IOTMAKERS_SDK_HOME)/lib
IOTMAKERS_SDK_INC_PATH = $(IOTMAKERS_SDK_HOME)/include
IOTMAKERS_SDK_LIB = $(IOTMAKERS_SDK_LIB_PATH)/$(IOTMAKERS_SDK_LIBNAME)

###########################################
# SYSLIB
###########################################
# Linux
SYSLIB = -lpthread

###########################################
# FLAGs
###########################################
INCLUDEALL = -I$(IOTMAKERS_SDK_INC_PATH)
CFLAGS = -W -O0 -g

LIBALL = $(IOTMAKERS_SDK_LIB) $(SYSLIB)
OFLAGS = -lwiringPi

###########################################
# SOURCE TREE
###########################################
C_OBJECT = \
	main.o 

###########################################
# BUILD
###########################################
PRODUCT = tt
all: $(PRODUCT)

$(PRODUCT) : $(C_OBJECT) 
	$(CC) -o $(PRODUCT) $(C_OBJECT) $(OFLAGS) $(LIBALL)

###########################################
# Compile
###########################################
.SUFFIXES : .o .asm .c .cxx .bas .sc .y .yxx .l .lxx  .pc

.c.o : $(C_SOURCE)
	$(CC) $(CFLAGS) $(INCLUDEALL) -c $*.c -o $*.o

.cxx.o : $(C_SOURCE)
	$(CC) $(CFLAGS) $(INCLUDEALL) -c $*.cxx -o $*.o

###########################################
# Util
###########################################
clean : $(C_SOURCE)
	rm -rf $(C_OBJECT)

