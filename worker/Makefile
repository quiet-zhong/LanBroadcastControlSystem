CC       = gcc -g
CXX      = g++
STRIP    = strip
MAKEXE   = make

CFLAGS	+= -fPIC -Wall
CFLAGS	+= -DDEBUG

COBJS		:= $(patsubst %.c,%.o,$(wildcard *.c))
CPPOBJS		:= $(patsubst %.cpp,%.o,$(wildcard *.cpp))

# INCLUDE  =   -I./ -I../include -I../lib/libusb

LOCAL_LIB_PATH 	= ../lib
# STATIC_LIBS 	+= $(LOCAL_LIB_PATH)/libusb.a

LINKPATH 	= -Wl,-rpath,./
LINKDIR		= ../lib
DYNAMIC_LIBS 	+= -lstdc++ -lpthread  -ldl -lm -lutil
DYNAMIC_LIBS 	+= -L$(LINKDIR)

TARGET  = ./worker

all: $(TARGET)
$(TARGET):$(COBJS) $(CPPOBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(INCLUDE) $(STATIC_LIBS) $(DYNAMIC_LIBS)
	$(STRIP) --strip-all $(TARGET)

$(COBJS) : %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

$(CPPOBJS): %.o : %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDE)

.PHONY:clean
clean:
	rm -rf *.o core $(COBJS) $(CPPOBJS) $(TARGET)



