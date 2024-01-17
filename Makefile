# target to create first shared library
libpoopl0gin.so: src/login.c
	gcc src/login.c -o libpoopl0gin.so -shared -fPIC

# target to create second shared library
libpoopinputch3ck.so: src/input_check.c
	gcc src/input_check.c -o libpoopinputch3ck.so -shared -fPIC

main.o: src/main.c
	gcc -c src/main.c 

all: libpoopl0gin.so libpoopinputch3ck.so main.o
	gcc main.o -I/src libpoopl0gin.so libpoopinputch3ck.so -o poop.out

clean:
	rm -rf *.so
	rm -rf *.o*