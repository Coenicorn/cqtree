EXEC := ./build/main.out

SRCDIR := ./src

BINDIR := ./bin
LIBDIR := ./lib

CFLAGS := -Wall -g -I$(SRCDIR)/ -I$(LIBDIR)/
LFLAGS := -L$(SRCDIR)/ -L$(BINDIR)/
LDFLAGS := -lm

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:.c=.o)

RM := rm -rf

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

$(EXEC): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $(EXEC) $(LDFLAGS)
	# $(MAKE) clean

build: $(EXEC)

clean:
	sudo $(RM) $(SRCDIR)/*.o