all :
	cd src && gcc -o ../main main.c -lglut -lGL -lGLU

clean:
	rm -f main
