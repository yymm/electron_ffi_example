compile:
	gcc -shared -fpic -lstdc++ delaunay.c -o libdelaunay.so

test:
	gcc -Wall delaunay.c test.c -o test -lm

clean:
	rm -f *.so *.out test
