CC       ?= cc
CFLAGS   ?= -std=c11 -Wall -Wextra -O2
CPPFLAGS ?= -Iinclude
LDFLAGS  ?=

TARGET := cfetch
SRC    := src/main.c src/info.c
OBJ    := $(SRC:.c=.o)
HDR    := include/info.h include/ascii.h

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

%.o: %.c $(HDR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean
