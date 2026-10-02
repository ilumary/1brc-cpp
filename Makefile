CXX := g++
TARGET := parser

BUILD ?= release
ifeq ($(BUILD),debug)
  OPT := -O0 -g3 -fno-omit-frame-pointer
  SANITIZE := -fsanitize=address,undefined
else
  OPT := -O3 -march=native -DNDEBUG -fno-omit-frame-pointer
  SANITIZE :=
endif

CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -Iinclude $(OPT) $(SANITIZE) -g -MMD -MP
LDFLAGS := $(SANITIZE)

BUILD_DIR := build/$(BUILD)
SRCS := main.cpp $(wildcard src/*.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all debug clean
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug:
	@$(MAKE) --no-print-directory BUILD=debug

clean:
	$(RM) -r build $(TARGET)

-include $(DEPS)
