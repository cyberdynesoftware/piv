OBJECTS = main.o Folder.o AnimatedGIF.o SingleView.o ScrollView.o Image.o WebpImage.o

CPP = g++
CPPFLAGS = -g -Wall -std=c++14

all: $(OBJECTS)
	$(CPP) $(OBJECTS) -o piv -pthread -lboost_filesystem -lwebp -lsfml-graphics -lsfml-window -lsfml-system

main.o: Stage.h SingleView.h ScrollView.h
Folder.o: Folder.h
SingleView.o: SingleView.h Stage.h Image.h
AnimatedGIF.o: AnimatedGIF.h AnimatedImage.h
ScrollView.o: ScrollView.h Stage.h Image.h
Image.o: Image.h AnimatedGIF.h
WebpImage.o: WebpImage.h AnimatedImage.h

clean:
	rm *.o
