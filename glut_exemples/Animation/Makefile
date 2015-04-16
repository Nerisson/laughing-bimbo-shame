LINKS = -lglut -lGLU -lGL -lm 
INCS = -I. -I ../../include
CC = gcc
SRCS    = .


PROG:	 Anime

FILES =	Anime

Anime:	Anime.c
	$(CC)	$@.c 	$(INCS)	$(LDPATH) $(LINKS)	-o $@

clean:
	cd $(SRCS); rm  $(FILES)
