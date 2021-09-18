OBJECTS = main.o Folder.o AnimatedGIF.o MultiImageView.o Image.o WebpImage.o

CPP = g++
CPPFLAGS = -g -Wall -std=c++14

all: $(OBJECTS)
	$(CPP) $(OBJECTS) -o piv -pthread -lboost_filesystem -lsfml-graphics -lsfml-window -lsfml-system -lwebp -lwebpdemux

main.o: SingleImageView.h MultiImageView.h Folder.h icon.h
Folder.o: Folder.h
MultiImageView.o: MultiImageView.h Image.h Folder.h font.h Help.h
Image.o: Image.h AnimatedGIF.h WebpImage.h AnimatedImage.h
AnimatedGIF.o: AnimatedGIF.h AnimatedImage.h
WebpImage.o: WebpImage.h AnimatedImage.h

install:
	cp piv /usr/local/bin/

clean:
	rm *.o
