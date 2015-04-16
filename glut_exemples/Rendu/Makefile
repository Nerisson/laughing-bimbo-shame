LINKS = -lglut -lGLU -lGL -lm 
INCS = -I. -I ../../include
CC = gcc
SRCS    = .
EXEC =	Flat \
	Damier\
	Cercle

PROG:	Flat \
	Damier\
	Cercle

FILES =	
Flat:	Flat.c
	$(CC)	$@.c 	$(INCS)	$(LDPATH) $(LINKS)	-o $@

Damier: Damier.c
	$(CC)	$@.c 	$(INCS)	$(LDPATH) $(LINKS)	-o $@


Cercle: Cercle.c
	$(CC)	$@.c 	$(INCS)	$(LDPATH) $(LINKS)	-o $@

clean:
	 rm  $(EXEC)
