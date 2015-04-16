LINKS = -lglut -lGLU -lGL -lm 
INCS = -I. -I ../../include
CC = gcc
SRCS    = .


PROG:	Resize \
        Viewports 


FILES =	Resize \
        Viewports 

Resize:	Resize.c
	$(CC)	$@.c 	$(INCS)	$(LDPATH) $(LINKS)	-o $@
Viewports:	Viewports.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@


clean:
	cd $(SRCS); rm  $(FILES)
