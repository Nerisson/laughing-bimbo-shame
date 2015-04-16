LINKS = -lglut -lGLU -lGL -lm -ltiff
INCS = -I. -I ../../include
CC = gcc
SRCS    = .


PROG:	Cube \
        Explode

FILES = Cube \
	 Explode


Cube:Cube.c
	$(CC)	$@.c 	$(INCS)	$(LDPATH) $(LINKS)	-o $@

Explode:Explode.c
	$(CC)	$@.c texture.c	$(INCS)	$(LDPATH) $(LINKS)	-o $@
clean:
	cd $(SRCS); rm  $(FILES)
