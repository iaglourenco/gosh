CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = main.c error_handling.c shell_functions.c
OBJS = $(SRCS:.c=.o)
TARGET = program

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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@