OUT := projet

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude

# GTKmm 4.0 (required for the GUI build)
GTKMM_CFLAGS := $(shell pkg-config --cflags gtkmm-4.0 2>/dev/null)
GTKMM_LIBS   := $(shell pkg-config --libs gtkmm-4.0 2>/dev/null)

SRC_DIR := src
BUILD_DIR := build

CXXFILES := $(wildcard $(SRC_DIR)/*.cc)
OFILES   := $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(CXXFILES))

.PHONY: all clean run test help check-deps

all: check-deps $(OUT)

help:
	@echo "Targets:"
	@echo "  make        Build the project"
	@echo "  make run    Run the program (optionally: make run TEST=tests/inputs/t27.txt)"
	@echo "  make test   Run automated (headless) test sweep"
	@echo "  make clean  Remove build artifacts"

check-deps:
	@if [ -z "$(GTKMM_CFLAGS)" ] || [ -z "$(GTKMM_LIBS)" ]; then \
		echo "Error: gtkmm-4.0 not found via pkg-config."; \
		echo "Install it:"; \
		echo "  - Debian/Ubuntu: sudo apt-get install -y libgtkmm-4.0-dev pkg-config"; \
		echo "  - macOS (Homebrew): brew install gtkmm4 pkg-config"; \
		echo "Then verify with: pkg-config --modversion gtkmm-4.0"; \
		exit 1; \
	fi

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(GTKMM_CFLAGS) $(OFILES) -o $@ $(GTKMM_LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(GTKMM_CFLAGS) -c $< -o $@

run: all
	@TEST_FILE=${TEST:-tests/inputs/t27.txt}; \
	echo "Running: ./$(OUT) $$TEST_FILE"; \
	LC_ALL=fr_FR.UTF-8 LANG=fr_FR.UTF-8 ./$(OUT) $$TEST_FILE

test: all
	@./scripts/run_all_tests.sh

clean:
	@echo "Removing build artifacts"
	@rm -rf $(BUILD_DIR) $(OUT) output.log tmp
