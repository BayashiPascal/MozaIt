OPTIONS_DEBUG=-ggdb -g3 -Wall
OPTIONS_RELEASE=-O3 
OPTIONS=$(OPTIONS_RELEASE)
INCPATH=/home/bayashi/Coding/Include
LIBPATH=/home/bayashi/Coding/Include

all : mozait

mozait: main.o mozait.o Makefile $(LIBPATH)/tgapaint.o $(LIBPATH)/gset.o
	gcc  $(OPTIONS) main.o $(LIBPATH)/tgapaint.o $(LIBPATH)/gset.o mozait.o -o mozait -lm

main.o : main.c mozait.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c main.c

mozait.o : mozait.c mozait.h $(INCPATH)/tgapaint.h $(INCPATH)/gset.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c mozait.c

clean : 
	rm -rf *.o mozait

test :
	./mozait test03.tga out.tga

valgrind :
	valgrind -v --track-origins=yes --leak-check=full --gen-suppressions=yes --show-leak-kinds=all ./mozait test01.tga out.tga
