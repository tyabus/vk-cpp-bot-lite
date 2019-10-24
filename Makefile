# vk-cpp-bot-lite Makefile

BOT_COMMIT = $(firstword $(shell git rev-parse --short=6 HEAD) unknown)
CFLAGS = -O2 -funsafe-loop-optimizations -Isrc/3rd_party -fomit-frame-pointer -std=c++11 -c -DCXX=\"$(CC)\" -DBOT_VERSION=\"$(BOT_COMMIT)\"
LDFLAGS = -lstdc++ -lcurl -pthread -lm
SOURCES = src/fs.cpp \
	  src/net.cpp \
	  src/vk.cpp \
	  src/lp.cpp \
	  src/str.cpp \
	  src/other.cpp \
	  src/msg.cpp \
	  src/modules.cpp \
	  src/cmd.cpp \
	  src/cmds.cpp \
       	  src/main.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = vkbotlite

ifeq ($(CC), cc)
	CC=g++
	CFLAGS+= -flto -Wno-psabi -march=native -funsafe-loop-optimizations
endif

ifdef TRAVIS
	CFLAGS+= -DTRAVIS -m32 -static
endif

ifdef NO_PYTHON
	CFLAGS+= -DNO_PYTHON
else
	CFLAGS+= $(shell pkg-config --cflags python3)
	LDFLAGS+= $(shell pkg-config --libs python3)
	LDFLAGS+= -lboost_python3
	SOURCES+= 	src/py.cpp
endif

ifdef DEBUG
	CFLAGS+= -g -ggdb -DDEBUG
endif

ifdef NO_LIBGD
	CFLAGS+= -DNO_LIBGD
else
	LDFLAGS+= -lgd
endif

LDFLAGS+= -Wl,-rpath

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)
	strip --strip-unneeded $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	strip --strip-unneeded $@
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
