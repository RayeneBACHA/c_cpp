run : compile
	./app

compile : clean 
	gcc main.c -o app
clean :
	rm -f app