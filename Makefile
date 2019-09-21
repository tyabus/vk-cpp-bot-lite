ARCH = $(shell uname -m)
BOT_COMMIT = $(firstword $(shell git rev-parse --short=6 HEAD) unknown)
CFLAGS = -O2 -std=c++11 -c -DCXX=\"$(CC)\" -DBOT_VERSION=\"$(BOT_COMMIT)\"
LDFLAGS = -lstdc++ -lcurl -pthread -lm
INCLUDES = -I thr/include -I json/include -I json/include/nlohmann
SOURCES = \
	src/fs.cpp \
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
EXECUTABLE = vkbotlite-$(ARCH)

ifeq ($(CC), gcc)
	CFLAGS+= -flto -Wno-psabi -march=native -funsafe-loop-optimizations
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
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
