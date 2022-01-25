all:
	gcc src/gameplay/*.c src/sat_gen/*.c main.c -o sprogue -lm -ggdb3