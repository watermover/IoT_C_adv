CC      = clang
CFLAGS  = -Wall -Wextra -O2
LDFLAGS = -lm

SRC = main.c mathfuncs.c
OBJ = $(SRC:.c=.o)
TARGET = area_prog

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c mathfuncs.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
