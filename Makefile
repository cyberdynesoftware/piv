OBJECTS = main.o Image.o Folder.o

CPP = g++
CPPFLAGS = -g -Wall -std=c++11

all: $(OBJECTS)
	$(CPP) $(OBJECTS) -o piv -pthread -lsfml-graphics -lsfml-window -lsfml-system

main.o: Image.h Folder.h
Image.o: Image.h
Folder.o: Folder.h Image.h

clean:
	rm *.o
