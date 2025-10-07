objects := board_utils.o place_tiles.o check.o
connect_four_waseda: $(objects)
	gcc main.c $^ -o connect_four_waseda -Werror -Wextra

$(objects): %.o: %.c
	gcc -c $^ -o $@

clean:
	rm -f *.o connect_four_waseda
