CFLAGS=-Ofast -std=c++11 -c
LDFLAGS=-lstdc++ -lcurl -lgd -pthread
INCLUDES= -I json/include -I json/include/nlohmann
SOURCES=	\
	fs.cpp \
	net.cpp \
	vk.cpp \
	lp.cpp \
	str.cpp \
	other.cpp \
	msg.cpp \
	modules.cpp \
	cmd.cpp \
	cmds.cpp \
	main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=vkbotlite

ifdef NO_PYTHON
	CFLAGS+= -DNO_PYTHON
else
	CFLAGS+= $(shell pkg-config --cflags python3)
	LDFLAGS+= $(shell pkg-config --libs python3)
	ifdef TERMUX
		LDFLAGS+= -lboost_python36
	else
		LDFLAGS+= -lboost_python-py35
	endif
	SOURCES+= 	py.cpp
endif

ifdef TERMUX
	LDFLAGS+= -latomic
endif

CFLAGS+= -Wno-psabi
LDFLAGS+= -Wl,-rpath

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)
	strip $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
