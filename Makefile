CC=gcc
CXX=g++
LD=gcc

CFLAGS = -Wall -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lglfw -lGLEW

SOURCE_DIR = source
BUILD_DIR = build
CPP_FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
OBJ_FILES := $(addprefix $(BUILD_DIR)/obj/,$(notdir $(CPP_FILES:.cpp=.o)))

default: Mars

$(BUILD_DIR)/obj/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)/obj
	$(CXX) $(CC_FLAGS) -c -o $@ $<

Mars: $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)


.PHONY: clean