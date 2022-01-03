############################################
# This is a project standard makefile..
############################################
SHELL	= /bin/sh
CC	= cc
CXX	= c++
AR	= ar rvs
RANLIB	= ranlib 

###########################################
# IOTMAKERS_SDK_HOME
###########################################
IOTMAKERS_SDK_HOME = ..
IOTMAKERS_SDK_LIB_PATH = $(IOTMAKERS_SDK_HOME)/lib
IOTMAKERS_SDK_LIBNAME = libiotmakers.a

###########################################
# FLAGs
###########################################
CFLAGS = -W -O0 -g \
	-D_LINUX_  -D_IM_C_SOCK_


INCLUDE  =  -I./ -I./third_party

###########################################
# SOURCE TREE
###########################################
HEADER = \

C_OBJECT = \
	./netio/sock_posix.o \
	./third_party/parson/parson.o \
	./util/log.o \
	./util/util.o \
	./util/thread.o \
	./packet/head_ext.o \
	./packet/head.o \
	./packet/body.o \
	./packet/packet.o \
	./packet/body_util.o \
	./packet/body_200.o \
	./packet/body_300.o \
	./packet/body_400.o \
	./packet/body_500.o \
	./packet/body_700.o \
	./action/if200_auth/if224_auth_channel_dev_tcp.o \
	./action/if200_auth/if231_auth_keepalive.o \
	./action/if300_device/if332_device_update.o \
	./action/if300_device/if333_device_info.o \
	./action/if400_collection/if411_collection_data.o \
	./action/if500_control/if525_control_data.o \
	./action/if700_query/if711_query_last.o \
	./action/recv_packet.o \
	./action/send_packet.o \
	./base/base.o \
	./iotmakers_api.o \







###########################################
# BUILD
###########################################
TARGET	= $(IOTMAKERS_SDK_LIB_PATH)/$(IOTMAKERS_SDK_LIBNAME)
all: $(TARGET)

$(TARGET): $(C_OBJECT)
	if [ -f $(TARGET) ]; then /bin/rm $(TARGET); fi;
	$(AR) $(TARGET) $(C_OBJECT) 

###########################################
# Compile
###########################################
.SUFFIXES : .o .asm .c .cxx .bas .sc .y .yxx .l .lxx  .pc

.c.o : $(C_SOURCE) $(PC_SOURCE)
	$(CC) $(CFLAGS) $(INCLUDE) -c $*.c -o $*.o

###########################################
# Util
###########################################
clean : $(C_SOURCE)
	rm -rf  $(C_OBJECT)



