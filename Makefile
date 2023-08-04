prog: libcclock.o
        gcc -shared -lm -o libcclock.so libcclock.o

libcclock.o: libcclock.cpp
        gcc -lm -c libcclock.cpp
