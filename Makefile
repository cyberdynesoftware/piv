OBJECTS = main.o Folder.o ImageCache.o AnimatedGIF.o ScrollView.o

CPP = g++
CPPFLAGS = -g -Wall -std=c++14

all: $(OBJECTS)
	$(CPP) $(OBJECTS) -o piv -pthread -lboost_filesystem -lsfml-graphics -lsfml-window -lsfml-system

main.o: ImageCache.h Stage.h SingleView.h
Folder.o: Folder.h
SingleView.o: SingleView.h Stage.h ImageCache.h
ImageCache.o: ImageCache.h Folder.h AnimatedGIF.h
AnimatedGIF.o: AnimatedGIF.h
ScrollView.o: ScrollView.h Stage.h ImageCache.h

clean:
	rm *.o
