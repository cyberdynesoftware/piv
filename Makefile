OBJECTS = main.o Image.o Folder.o SingleView.o

CPP = g++
CPPFLAGS = -g -Wall -std=c++11

all: $(OBJECTS)
	$(CPP) $(OBJECTS) -o piv -pthread -lboost_filesystem -lsfml-graphics -lsfml-window -lsfml-system

main.o: Image.h Folder.h Stage.h
Image.o: Image.h
Folder.o: Folder.h Image.h
SingleView.o: SingleView.h Stage.h Folder.h

clean:
	rm *.o
