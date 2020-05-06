OBJECTS = main.o Image.o Folder.o SingleView.o ImageCache.o

CPP = g++
CPPFLAGS = -g -Wall -std=c++11

all: $(OBJECTS)
	$(CPP) $(OBJECTS) -o piv -pthread -lboost_filesystem -lsfml-graphics -lsfml-window -lsfml-system

main.o: ImageCache.h Stage.h SingleView.h
Image.o: Image.h
Folder.o: Folder.h
SingleView.o: SingleView.h Stage.h ImageCache.h
ImageCache.o: ImageCache.h Image.h Folder.h

clean:
	rm *.o
