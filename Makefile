run: compile
	./myapp

compile: rm
	gcc main.c -o myapp

rm:
	rm ./myapp