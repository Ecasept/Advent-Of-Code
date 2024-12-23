# Variables
CC = gcc
CXX = g++
CFLAGS_BASE = 
CFLAGS_EXTRA = -Wall -Wextra -Wpedantic -Werror -lm
BUILD_DIR = build
OUTPUT = 

# Enable debug symbols if DEBUG is set to true, otherwise optimize
ifeq ($(DEBUG),true)
    CFLAGS_BASE += -g3 -O0 -fdiagnostics-color=always
else
	CFLAGS_BASE += -O2
endif

# Phony targets
.PHONY: clean day% run% test FORCE

default: test

vscode_Day\ %: day% FORCE ;
FORCE:

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
		echo -e "\033[1mmake\033[0m: Compiling \033[1mDay $*/solution.c\033[0m with $(CC)..."; \
		if [ $* -gt 6 ]; then \
			if [ $* -eq 17 ]; then \
				$(CC) $$CFLAGS -DSTRING_RESULT_PART1 lib/runner.c "Day $*/solution.c" lib/cutils.c -o $(BUILD_DIR)/$${OUTPUT_NAME}; \
			else \
				$(CC) $$CFLAGS lib/runner.c "Day $*/solution.c" lib/cutils.c -o $(BUILD_DIR)/$${OUTPUT_NAME}; \
			fi; \
		else \
			$(CC) $$CFLAGS lib/runner.c "Day $*/solution.c" -o $(BUILD_DIR)/$${OUTPUT_NAME}; \
		fi; \
	elif [ -f "Day $*/solution.cpp" ]; then \
		echo -e "\033[1mmake\033[0m: Compiling \033[1mDay $*/solution.cpp\033[0m with $(CXX)..."; \
		if [ $* -eq 18 ] || [ $* -eq 23 ]; then \
			$(CXX) $$CFLAGS -DSTRING_RESULT_PART2 lib/runner.cpp "Day $*/solution.cpp" lib/utils.cpp -o $(BUILD_DIR)/$${OUTPUT_NAME}; \
		else \
			$(CXX) $$CFLAGS lib/runner.cpp "Day $*/solution.cpp" lib/utils.cpp -o $(BUILD_DIR)/$${OUTPUT_NAME}; \
		fi; \
	else \
		echo -e "\033[1mmake\033[0m: \033[31mError: No solution.c or solution.cpp found in Day$*\033[0m"; \
		exit 1; \
	fi


# Clean up the build directory
clean:
	rm -rf $(BUILD_DIR)

run%: day%
	@cd "Day $*" && ../$(BUILD_DIR)/solution$* $(ARGS)

test:
	@python3 test/test.py
