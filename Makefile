# Temporary solution until it is changed to autoconf
CC = gcc
CFLAGS += -Wall -Wextra -O0 -g -ggdb -pipe

whiteboard: main.o wii.o matrix.o
	${CC} ${CFLAGS}	main.o wii.o matrix.o -o whiteboard \
		-lcwiid `pkg-config --libs xtst` `sdl-config --libs`

clean:
	rm -vf whiteboard release.tar.gz *.o

main.o: main.c matrix.h 
	${CC} ${CFLAGS} -c main.c `sdl-config --cflags`

wii.o: wii.c matrix.h 
	${CC} ${CFLAGS} -c wii.c

matrix.o: matrix.c
	${CC} ${CFLAGS} -c matrix.c

release: whiteboard README.txt TODO
	tar -zcvf release.tar.gz whiteboard README.txt TODO
