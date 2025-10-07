connect_four_waseda: board_utils
	gcc main.c board_utils.o place_tiles.o check.o -o connect_four_waseda -Werror -Wextra

board_utils:
	gcc -c board_utils.c -o board_utils.o
	gcc -c place_tiles.c -o place_tiles.o
	gcc -c check.c -o check.o

clean:
	rm -f board_utils.o place_tiles.o check.o
