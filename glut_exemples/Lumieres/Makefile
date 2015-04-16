LINKS = -lglut -lGLU -lGL -lm 
INCS = -I. -I ../../include
CC = gcc
SRCS    = .


PROG:	 PremiereNotion \
         Surface 


FILES =	 PremiereNotion \
         Surface 



PremiereNotion:	PremiereNotion.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@

Surface : Surface.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@

clean:
	cd $(SRCS); rm  $(FILES)
