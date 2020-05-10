OBJECTS = main.o Folder.o SingleView.o ImageCache.o AnimatedGIF.o

CPP = g++
CPPFLAGS = -g -Wall -std=c++14

all: $(OBJECTS)
	$(CPP) $(OBJECTS) -o piv -pthread -lboost_filesystem -lsfml-graphics -lsfml-window -lsfml-system

main.o: ImageCache.h Stage.h SingleView.h
Folder.o: Folder.h
SingleView.o: SingleView.h Stage.h ImageCache.h
ImageCache.o: ImageCache.h Folder.h AnimatedGIF.h
AnimatedGIF.o: AnimatedGIF.h

clean:
	rm *.o
