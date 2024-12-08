# Variables
CC = gcc
CXX = g++
CFLAGS_BASE = 
CFLAGS_EXTRA = -Wall -Wextra -Wpedantic -std=c11
BUILD_DIR = build
OUTPUT = 

# Enable debug symbols if DEBUG is set to true, otherwise optimize
ifeq ($(DEBUG),true)
    CFLAGS_BASE += -g -fdiagnostics-color=always
else
	CFLAGS_BASE += -O2
endif

# Phony targets
.PHONY: all clean day% run% test

default: all

# Target to compile a specific day's solution
day%:
	@mkdir -p $(BUILD_DIR) # Ensure build directory exists
	@OUTPUT_NAME=$${OUTPUT:-solution$*}; \
	if [ $* -gt 7 ]; then \
		CFLAGS="$(CFLAGS_BASE) $(CFLAGS_EXTRA)"; \
	else \
		CFLAGS="$(CFLAGS_BASE)"; \
	fi; \
	\
	if [ -f "Day $*/solution.c" ]; then \
		echo "Compiling Day $*/solution.c with gcc..."; \
		if [ $* -gt 6 ]; then \
			$(CC) $$CFLAGS lib/runner.c "Day $*/solution.c" lib/cutils.c -o $(BUILD_DIR)/$${OUTPUT_NAME}; \
		else \
			$(CC) $$CFLAGS lib/runner.c "Day $*/solution.c" -o $(BUILD_DIR)/$${OUTPUT_NAME}; \
		fi; \
	elif [ -f "Day $*/solution.cpp" ]; then \
		echo "Compiling Day $*/solution.cpp with g++..."; \
		$(CXX) $$CFLAGS lib/runner.cpp "Day $*/solution.cpp" lib/utils.cpp -o $(BUILD_DIR)/$${OUTPUT_NAME}; \
	else \
		echo "Error: No solution.c or solution.cpp found in Day$*"; \
		exit 1; \
	fi


# Clean up the build directory
clean:
	rm -rf $(BUILD_DIR)

all: day1 day2 day3 day4 day5 day6 day7

run%: day%
	@cd "Day $*" && ../$(BUILD_DIR)/solution$* $(ARGS)

test:
	@python3 test/test.py
