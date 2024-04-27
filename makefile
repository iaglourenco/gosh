CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = gosh.c error_handling.c shell_functions.c
OBJS = $(SRCS:.c=.o)
TARGET = gosh

# cat
CAT_SRCS = cat/cat.c
CAT_OBJS = $(CAT_SRCS:.c=.o)
CAT_TARGET = cat/cat

# ls
LS_SRCS = ls/ls.c
LS_OBJS = $(LS_SRCS:.c=.o)
LS_TARGET = ls/ls


.PHONY: all clean cat ls

all: $(TARGET) $(CAT_TARGET) $(LS_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(CAT_TARGET): $(CAT_OBJS)
	$(CC) $(CFLAGS) $(CAT_OBJS) -o $(CAT_TARGET)

$(LS_TARGET): $(LS_OBJS)
	$(CC) $(CFLAGS) $(LS_OBJS) -o $(LS_TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(CAT_OBJS) $(CAT_TARGET) $(LS_OBJS) $(LS_TARGET)
	rm -rf release/

release: CFLAGS := $(filter-out -g,$(CFLAGS))
release: all
	mkdir -p release
	zip -r release/gosh.zip gosh cat/cat ls/ls

setup:
	sudo apt-get update
	sudo apt-get install -y build-essential


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@