space_game: main.c src/sat_gen.c src/draw.c src/input.c src/room_type_load.c
	gcc main.c src/sat_gen.c src/draw.c src/input.c src/room_type_load.c -o space_game -lm -lcurses -ggdb3
win_space_game: main.c src/sat_gen.c src/draw.c src/input.c src/room_type_load.c
	gcc main.c src/sat_gen.c src/draw.c src/input.c src/room_type_load.c -o win_space_game_64 -lm -ggdb3
