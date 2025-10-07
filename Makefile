EXEC := connect_four_waseda

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# Make executable ./build/connect_four_waseda from objects
$(BUILD_DIR)/$(EXEC): $(OBJS)
	gcc $(OBJS) -o $@ -Werror -Wextra

# Find all files that end in .c in ./build and make .o's out of them
# and then dump them into a subdirectory named after the one they came from
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	gcc -c $< -o $@

# Nukes ./build
clean:
	rm -r $(BUILD_DIR)
