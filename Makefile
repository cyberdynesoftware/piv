OBJECTS = main.o Folder.o ImageCache.o AnimatedGIF.o SingleView.o ScrollView.o ImageData.o Image.o

CPP = g++
CPPFLAGS = -g -Wall -std=c++14

all: $(OBJECTS)
	$(CPP) $(OBJECTS) -o piv -pthread -lboost_filesystem -lOpenImageIO -lsfml-graphics -lsfml-window -lsfml-system

main.o: ImageCache.h Stage.h SingleView.h ScrollView.h ImageCache.h ImageData.h AnimatedGIF.h
Folder.o: Folder.h
SingleView.o: SingleView.h Stage.h ImageCache.h ImageData.h Image.h
ImageCache.o: ImageCache.h Folder.h ImageData.h
AnimatedGIF.o: AnimatedGIF.h
ScrollView.o: ScrollView.h Stage.h ImageCache.h ImageData.h
ImageData.o: ImageData.h AnimatedGIF.h
Image.o: Image.h

clean:
	rm *.o
