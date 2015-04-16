LINKS = -lglut -lGLU -lGL -lm 
INCS = -I. -I ../../include
CC = gcc
SRCS    = .


PROG:	 BSplineCurve\
         BSplineSurface \
         TrimmedSurface
  
     
FILES =	BSplineCurve\
         BSplineSurface \
	TrimmedSurface
  
  
BSplineCurve:	BSplineCurve.c
	$(CC)	$@.c 	$(INCS)	$(LDPATH) $(LINKS)	-o $@

BSplineSurface:	BSplineSurface.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@
TrimmedSurface:	TrimmedSurface.c
	$(CC)	$@.c 	$(INCS) $(LDPATH) $(LINKS)	-o $@
clean:
	cd $(SRCS); rm  $(FILES)
