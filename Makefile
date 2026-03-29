CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?=
LDFLAGS ?=
LDLIBS ?=

SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/xsh
TEST_DIR := tests
TEST_BUILD_DIR := $(BUILD_DIR)/tests
TEST_PARSE_BIN := $(TEST_BUILD_DIR)/test_parse
TEST_BUILTINS_BIN := $(TEST_BUILD_DIR)/test_builtins
TEST_SHELL_BIN := $(TEST_BUILD_DIR)/test_shell

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean run rebuild test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

rebuild: clean all

test: $(TEST_PARSE_BIN) $(TEST_BUILTINS_BIN) $(TEST_SHELL_BIN)
	./$(TEST_PARSE_BIN)
	./$(TEST_BUILTINS_BIN)
	./$(TEST_SHELL_BIN)

$(TEST_PARSE_BIN): $(TEST_DIR)/test_parse.c $(SRC_DIR)/parse.c $(SRC_DIR)/parse.h | $(TEST_BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(SRC_DIR) $< $(SRC_DIR)/parse.c $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_BUILTINS_BIN): $(TEST_DIR)/test_builtins.c $(SRC_DIR)/builtins.c $(SRC_DIR)/builtins.h $(SRC_DIR)/parse.h | $(TEST_BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(SRC_DIR) $< $(SRC_DIR)/builtins.c $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_SHELL_BIN): $(TEST_DIR)/test_shell.c $(SRC_DIR)/shell.c $(SRC_DIR)/shell.h $(SRC_DIR)/parse.h $(SRC_DIR)/builtins.h | $(TEST_BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(SRC_DIR) $< $(SRC_DIR)/shell.c $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_BUILD_DIR): | $(BUILD_DIR)
	mkdir -p $(TEST_BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) xsh

-include $(DEPS)
