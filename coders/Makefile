NAME=codexion
CC=gcc
DEPS=codexion.h
CFILES=codexion.c helper.c minheap_implementation.c monitor.c\
		parsing.c process.c routine.c thread_coder.c
OFILES=$(CFILES:.c=.o)

AOUT ?= 4 35 10 10 10 3 5 fifo

all: $(NAME)

$(NAME): $(CFILES) $(DEPS)
	$(CC) $^ -o $@

clean:
	rm -f $(OFILES)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

run: 
	./codexion $(AOUT)

debug: $(CFILES) $(DEPS)
	$(CC) -fsanitize=thread -g -fno-sanitize=thread $^ -o $(NAME)

.PHONY: all clean fclean re