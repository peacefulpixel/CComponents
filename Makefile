CC      = gcc
CFLAGS  = -O3 -c -Wall -fPIC
SRC_DIR = src
BIN_DIR = bin
TST_DIR = test
BUILD_DIR = build
COMPSRC = $(SRC_DIR)/list.c $(SRC_DIR)/map.c $(SRC_DIR)/string.c
COMPOBJ = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(COMPSRC))
SOURCES = $(COMPSRC)
OBJECTS = $(COMPOBJ)
SOURCES_TST = $(TST_DIR)/tests/list.c $(TST_DIR)/tests/map.c $(TST_DIR)/tests/string.c 
OBJECTS_TST = $(patsubst $(TST_DIR)/%.c, $(TST_DIR)/%.o, $(SOURCES_TST))
TEST_SRC = $(TST_DIR)/test.c
TEST_OBJ = $(TST_DIR)/test.o
TEST_EXE = $(TST_DIR)/run_tests
LIB      = libccomponents

# Set to null to disable debug and @ to enable. Works also for TST
LOG = 
TST = 

all: $(BIN_DIR) $(OBJECTS) test $(LIB)

$(BIN_DIR):
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mCreating \033[1mbin\033[0m\033[0;34m directory:\033[0m\n"; fi
	$(LOG)mkdir $(BIN_DIR)

$(BIN_DIR)/%.o : $(SRC_DIR)/%.c
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mCompiling \033[1m$^\033[0m\033[0;34m source file:\033[0m\n"; fi
	$(LOG)$(CC) $(CFLAGS) $< -o $@

$(TST_DIR)/%.o : $(TST_DIR)/%.c
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mBuilding test \033[1m$^:\033[0m\n"; fi
	$(LOG)$(CC) $(CFLAGS) $< -o $@

test: $(TEST_OBJ) $(OBJECTS_TST) $(OBJECTS_SYS) $(COMPOBJ)
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mBuilding \033[1mtest application:\033[0m\n"; fi
	$(LOG)$(CC) $(OBJECTS) $(OBJECTS_TST) $(TEST_OBJ) -o $(TEST_EXE)
	@if [ ! $$(echo -n $(TST) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mRunning \033[1mtests:\033[0m\n"; fi
	@if [ ! $$(echo -n $(TST) | wc -m) -ge 1 ]; then \
		./$(TEST_EXE)          ; else \
		./$(TEST_EXE) --nologs ; fi
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mDeleting \033[1mtests object files:\033[0m\n"; fi
	$(LOG)rm -f $(OBJECTS_TST) $(TEST_OBJ) $(TEST_EXE)

$(LIB): $(OBJECTS)
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mCreating \033[1mbuild\033[0m\033[0;34m directory:\033[0m\n"; fi
	$(LOG)mkdir $(BUILD_DIR)
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mBuilding \033[1mlibrary:\033[0m\n"; fi
	$(LOG)$(CC) -Wall -shared -o $(BUILD_DIR)/$(LIB).so $(OBJECTS)
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mCopying \033[1mlibrary header:\033[0m\n"; fi
	$(LOG)cp $(SRC_DIR)/ccomponents.h $(BUILD_DIR)

clean:
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mRecursive removing \033[1mbin\033[0m\033[0;34m directory:\033[0m\n"; fi
	$(LOG)rm -rf bin
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mRemoving \033[1mtests\033[0m\033[0;34m symlink:\033[0m\n"; fi
	$(LOG)rm -f $(OBJECTS_TST) $(TEST_OBJ) $(TEST_EXE)
	@if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mRecursive removing \033[1mbuild\033[0m\033[0;34m directory:\033[0m\n"; fi
	$(LOG)rm -rf build

rebuild: clean all