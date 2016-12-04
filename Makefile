all: shell

shell: shell.c
	gcc -o shell -std=c11 shell.c

clean:
	rm -f shell
