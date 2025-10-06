connect_four_waseda: board_utils
	gcc main.c board_utils.o -o connect_four_waseda -Werror -Wextra

board_utils:
	gcc -c board_utils.c -o board_utils.o

clean:
	rm -f board_utils.o
