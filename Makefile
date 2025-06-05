run: compile
	./v1

compile: clean
	gcc izr.c -o v1

clean:
	rm -f v1