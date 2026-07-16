APP := build/launch_game
BUILD_DIR := build

CXX := g++
CXXSTD := c++20
CXXFLAGS := -std=$(CXXSTD) -Wall -Wextra -Wpedantic -O2 -MMD -MP
CPPFLAGS := -Iinclude
LDFLAGS :=
LDLIBS := -lSDL3 -lSDL3_image

SRC := main.cpp $(wildcard src/*.cpp)
OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

.PHONY: build clean rebuild

build: $(APP)

$(APP): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(APP)

rebuild: clean
	$(MAKE) build

-include $(DEP)
