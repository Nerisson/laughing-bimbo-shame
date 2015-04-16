LINKS = -lglut -lGLU -lGL -lm -ltiff
INCS = -I. -I ../../include
CC = gcc
SRCS    = .


PROG:	 CourbeBezier\
         SurfaceBezier 


FILES =	CourbeBezier\
         SurfaceBezier 


CourbeBezier:	CourbeBezier.c
	$(CC)	$@.c 	$(INCS)	$(LDPATH) $(LINKS)	-o $@

SurfaceBezier:	SurfaceBezier.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@



clean:
	cd $(SRCS); rm  $(FILES)
