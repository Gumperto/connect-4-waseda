EXEC := connect_four_waseda

BUILD_DIR := ./build
LEADERBOARD_FILES := $(BUILD_DIR)/leaderboard.txt $(BUILD_DIR)/player_list.txt
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP -Werror -Wextra

# Make executable ./build/connect_four_waseda from objects
$(BUILD_DIR)/$(EXEC): $(OBJS) $(LEADERBOARD_FILES)
	gcc $(OBJS) -o $@ $(LDFLAGS) -lncurses -lpanel -lmenu

# Find all files that end in .c in ./build and make .o's out of them
# and then dump them into a subdirectory named after the one they came from
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	gcc $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LEADERBOARD_FILES):
	touch $@

# Nukes ./build
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
