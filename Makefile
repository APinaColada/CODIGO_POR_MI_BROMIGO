all:
	cc -D_DEFAULT_SOURCE -Wall -std=c99 -g database.c driver.c -o driver

grind:
	valgrind --leak-check=full --track-origins=yes ./driver

clean:
	rm -rf driver driver.dSYM

