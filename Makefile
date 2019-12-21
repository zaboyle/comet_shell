CC=gcc -g -Wall

# Test source code to compile
TEST=test_prog.c

# List of source files for shell
SHELL_SOURCES=shell.c

# Generate the names of the shell's object files
SHELL_OBJS=${SHELL_SOURCES:.cpp=.o}

all: shell test

# Compile the shell
shell: ${SHELL_OBJS}
	${CC} -o $@ $^

# Compile test program
test: ${TEST}
	${CC} -o $@ $^


# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${SHELL_OBJS} shell test
