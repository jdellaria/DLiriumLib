#Makefile that builds SenseHat library.

LIBRARY_NAME = libDLiriumLib

# Prefix
PREFIX = /usr

# List of source files

SOURCES = $(wildcard *.cpp)

# List of object files
OBJECTS += $(SOURCES:.cpp=.o)

# Headers
HEADERS += $(SOURCES:.cpp=.h)

# Define C++ compiler flags
#override CXXFLAGS = -O3 -Wall
override CXXFLAGS = -Wall -fPIC -std=c++0x

# Linker flags
LDFLAGS = -shared

all: $(LIBRARY_NAME).so

# Compile all of the source files
%.o: %.cpp
	@echo Compiling file: $<
	g++ $(CXXFLAGS) -c $< -o $@
	@echo

# Build the binary
$(LIBRARY_NAME).so: $(OBJECTS)
	@echo Building target file: $@.
	g++ $(LDFLAGS)  -o $@ $^ -lm


# Install the library in $(prefix)
install:
	install -m 0644 $(LIBRARY_NAME).so $(PREFIX)/lib
	install -m 0644 Release/$(LIBRARY_NAME).a $(PREFIX)/lib
	install -m 0644 $(HEADERS) $(PREFIX)/include

# Uninstall the library from the system
uninstall:
	rm -f $(PREFIX)/lib/$(LIBRARY_NAME).so
	rm -f $(PREFIX)/lib/$(LIBRARY_NAME).a
# this renoves the first h file and the rest from the current directory	rm -f $(PREFIX)/include/$(HEADERS)

.PHONY: clean
clean:
	rm $(LIBRARY_NAME).so $(OBJECTS)
