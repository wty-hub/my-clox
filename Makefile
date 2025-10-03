# Makefile for clox interpreter

# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99 -O2
DEBUG_FLAGS = -g -DDEBUG -O0
TARGET = clox

# Source files
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)

# Object files
OBJECTS = $(SOURCES:.c=.o)
# Put object files in a separate directory
OBJDIR := build

# Override OBJECTS to live under $(OBJDIR)
OBJECTS := $(patsubst %.c,$(OBJDIR)/%.o,$(SOURCES))

# Pattern rule to produce objects under $(OBJDIR)
# ensures the directory exists before compiling
$(OBJDIR)/%.o: %.c $(HEADERS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Build object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


#define DEBUG_PRINT_CODE
#define DEBUG_TRACE_EXECUTION

# Debug build
debug: CFLAGS = -Wall -std=c99 -O0 $(DEBUG_FLAGS)
debug: $(TARGET)

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rebuild everything
rebuild: clean all

# Install (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  clean    - Remove build files"
	@echo "  rebuild  - Clean and build"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  uninstall- Remove from /usr/local/bin"
	@echo "  run      - Build and run the program"
	@echo "  help     - Show this help message"

# Declare phony targets
.PHONY: all debug clean rebuild install uninstall run help
