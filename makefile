CFLAGS = -std=c++17 -Iinclude -Wall -Wextra -Wpedantic 

SRC_DIR = src
BUILD_DIR = build
SOURCES = $(wildcard $(shell find $(SRC_DIR) -name "*.cpp"))
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o, $(SOURCES))

TARGET = $(BUILD_DIR)/output

run: $(TARGET)
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
