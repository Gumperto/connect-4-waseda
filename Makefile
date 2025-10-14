EXEC := connect_four_waseda

BUILD_DIR := ./build
LEADERBOARD_FILES := $(BUILD_DIR)/leaderboard.txt $(BUILD_DIR)/player_list.txt
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# Make executable ./build/connect_four_waseda from objects
$(BUILD_DIR)/$(EXEC): $(OBJS) $(LEADERBOARD_FILES)
	gcc $(OBJS) -o $@ -Werror -Wextra -lncurses -lpanel -lmenu

# Find all files that end in .c in ./build and make .o's out of them
# and then dump them into a subdirectory named after the one they came from
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	gcc -c $< -o $@

$(LEADERBOARD_FILES):
	touch $@

# Nukes ./build
clean:
	rm -r $(BUILD_DIR)
