CC = g++

CFLAGS = -g -Wall 
INCFLAGS = -I./glm-0.9.2.7 -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lglut -lGLU -lGL -lX11 -lfreeimage

RM = /bin/rm -f 
all: raytrace

raytrace: main.o  Transform.o readfile.o hw4.o variables.h readfile.h 
	$(CC) $(CFLAGS) -o raytrace main.o Transform.o readfile.o hw4.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
readfile.o: readfile.cpp readfile.h variables.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
Transform.o: Transform.cpp Transform.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp  
grader.o: UCSD/grader.cpp UCSD/grader.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c UCSD/grader.cpp
hw4.o: hw4.cpp hw4.hpp 
	$(CC) $(CFLAGS) $(INCFLAGS) -c hw4.cpp  
clean: 
	$(RM) *.o raytrace *.png


 
