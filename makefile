space_game: main.c src/sat_gen/sat_gen.c src/gameplay/draw.c src/gameplay/input.c src/sat_gen/room_type_load.c
	gcc main.c src/sat_gen/sat_gen.c src/gameplay/draw.c src/gameplay/input.c src/sat_gen/room_type_load.c -o space_game -lm -ggdb3

combat:
	gcc src/gameplay/cards.c src/gameplay/enemies.c src/gameplay/combat.c -o combat -lm -ggdb3