#
# $Id: Makefile 20130626 2013-06-26 22:10:10Z riddick $
#


CPP = g++

%.o:%.cpp
	$(CPP) -c $< -o $@

OBJS_BASE = FindPath.a

OBJS = demo.cpp FindPath.h map.h

demo:
	$(CPP) $(OBJS) $(OBJS_BASE) -o finder

makelib:
	$(CPP) -c -o FindPath.a FindPath.cpp 



clean:
	$(RM) -rf *.o

