LINKS = -lglut -lGLU -lGL -lm 
INCS = -I. -I ../../include
CC = gcc
SRCS    = .


PROG:	 Lookat \
         PolarView

FILES =	Lookat \
         PolarView


Lookat:	Lookat.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@

PolarView:	PolarView.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@

clean:
	cd $(SRCS); rm  $(FILES)
