output: Source.o libmylib.a
	gcc -lm -o output Source.o -L. -lmylib

Source.o: Source.cpp
	gcc -O -c Source.cpp

SearchDog.o: SearchDog.cpp SearchDog.h
	gcc -O -c SearchDog.cpp

libmylib.a: SearchDog.o
	ar rcs libmylib.a SearchDog.o

libs: libmylib.a

clean:
	rm -f myProgram *.o *.a *.gch