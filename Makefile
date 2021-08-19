OBJECTS = main.o Folder.o AnimatedGIF.o SingleImageView.o MultiImageView.o Image.o WebpImage.o

CPP = g++
CPPFLAGS = -g -Wall -std=c++14

all: $(OBJECTS)
	$(CPP) $(OBJECTS) -o piv -pthread -lboost_filesystem -lsfml-graphics -lsfml-window -lsfml-system -lwebp -lwebpdemux

main.o: ImageView.h SingleImageView.h MultiImageView.h Folder.h Icon.h
Folder.o: Folder.h
SingleImageView.o: SingleImageView.h ImageView.h Image.h Folder.h
MultiImageView.o: MultiImageView.h ImageView.h Image.h Folder.h
Image.o: Image.h AnimatedGIF.h WebpImage.h AnimatedImage.h
AnimatedGIF.o: AnimatedGIF.h AnimatedImage.h
WebpImage.o: WebpImage.h AnimatedImage.h

clean:
	rm *.o
