CXX:=g++
CXXFLAGS:= -Wall -Wextra -pedantic -std=c++17 -Wconversion -Wshadow -Wunused -Wuninitialized -g
TARGET_MAIN:=differentiator
TARGET_TEST:=test

SRC_DIR:=src
BUILD_DIR:=build
TEST_DIR:=testing

SRCS:=$(wildcard $(SRC_DIR)/*.cpp)
OBJS:=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o, $(SRCS))

.PHONY: all build run clean test test_build

all: build

test_build: $(BUILD_DIR)/$(TARGET_TEST)

test: test_build
	@./$(BUILD_DIR)/$(TARGET_TEST)

build: $(BUILD_DIR)/$(TARGET_MAIN)

run: build
	@./$(BUILD_DIR)/$(TARGET_MAIN)

clean:
	@echo "<Cleaning>"
ifeq ($(OS),Windows_NT)
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
else
	rm -rf $(BUILD_DIR)
endif


$(BUILD_DIR)/$(TARGET_TEST): $(TEST_DIR)/test.cpp $(OBJS)
ifeq ($(OS),Windows_NT)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
else
	mkdir -p $(BUILD_DIR)
endif
	@$(CXX) $(CXXFLAGS) $^ -o $@


$(BUILD_DIR)/$(TARGET_MAIN): $(TARGET_MAIN).cpp $(OBJS)
ifeq ($(OS),Windows_NT)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
else
	mkdir -p $(BUILD_DIR)
endif
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(OS),Windows_NT)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
else
	mkdir -p $(BUILD_DIR)
endif
	@$(CXX) $(CXXFLAGS) -c $< -o $@
