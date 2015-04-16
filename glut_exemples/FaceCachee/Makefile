LINKS = -lglut -lGLU -lGL -lm 
INCS = -I. -I ../../include
CC = gcc
SRCS    = .


PROG:	 NoHiddenFaces \
         Culling 


FILES =	  NoHiddenFaces \
          Culling



NoHiddenFaces:	NoHiddenFaces.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@

Culling:	Culling.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@
clean:
	cd $(SRCS); rm  $(FILES)
