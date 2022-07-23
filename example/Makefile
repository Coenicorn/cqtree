EXEC := ./build/main.out

SRCDIR := ./src

BINDIR := ./bin
LIBDIR := ./lib

CFLAGS := -Wall -g -I$(SRCDIR)/ -I$(LIBDIR)/ -O4
LFLAGS := -L$(SRCDIR)/ -L$(BINDIR)/
LDFLAGS := -lastar -lqtree -lraylib -lm -lGL -lm -lpthread -ldl -lrt

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:.c=.o)

RM := rm -rf

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

build: $(OBJS)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $(EXEC) $(LDFLAGS)
	$(MAKE) clean

clean:
	sudo $(RM) $(SRCDIR)/*.o