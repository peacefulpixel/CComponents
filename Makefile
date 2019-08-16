CC      = gcc
CFLAGS  = -O3 -ansi -std=c11 -c -Wall -fPIC
CFLAGST = -O3 -ansi -std=c11 -Wall
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
TEST_EXE = test.tcl
LIB      = libccomponents

MSG_BEG = @if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then printf "\033[0;35m\033[1m[CComponents]\033[0m 
MSG_END = "; fi

# Set to null to disable debug and @ to enable. Works also for TST
LOG = 
TST = 

all: $(BIN_DIR) $(OBJECTS) test $(LIB)

$(BIN_DIR):
	$(MSG_BEG)\033[0m\033[0;34mCreating \033[1mbin\033[0m\033[0;34m directory:\033[0m\n$(MSG_END)
	$(LOG)mkdir $(BIN_DIR)

$(BIN_DIR)/%.o : $(SRC_DIR)/%.c
	$(MSG_BEG)\033[0m\033[0;34mCompiling \033[1m$^\033[0m\033[0;34m source file:\033[0m\n$(MSG_END)
	$(LOG)$(CC) $(CFLAGS) $< -o $@

$(TST_DIR)/%.o : $(TST_DIR)/%.c
	$(MSG_BEG)\033[0m\033[0;34mBuilding test \033[1m$^:\033[0m\n$(MSG_END)
	$(LOG)$(CC) $(CFLAGST) $< -o $@ $(COMPOBJ)

test: $(TEST_OBJ) $(OBJECTS_TST) $(COMPOBJ)
	@if [ ! $$(echo -n $(TST) | wc -m) -ge 1 ]; then \
		printf "\033[0m\033[0;34mRunning \033[1mtests:\033[0m\n"; fi
	@if [ ! $$(echo -n $(TST) | wc -m) -ge 1 ]; then \
		cd test && ./$(TEST_EXE) -debug ; else \
		cd test && ./$(TEST_EXE) -nout  ; fi
	$(MSG_BEG)\033[0m\033[0;34mDeleting \033[1mtests object files:\033[0m\n"; fi
	$(LOG)rm -f $(OBJECTS_TST) $(TEST_OBJ)

$(LIB): $(OBJECTS)
	$(MSG_BEG)\033[0m\033[0;34mCreating \033[1mbuild\033[0m\033[0;34m directory:\033[0m\n$(MSG_END)
	$(LOG)mkdir $(BUILD_DIR)
	$(MSG_BEG)\033[0m\033[0;34mBuilding \033[1mlibrary:\033[0m\n$(MSG_END)
	$(LOG)$(CC) -Wall -shared -o $(BUILD_DIR)/$(LIB).so $(OBJECTS)
	$(MSG_BEG)\033[0m\033[0;34mCopying \033[1mlibrary header:\033[0m\n$(MSG_END)
	$(LOG)cp $(SRC_DIR)/ccomponents.h $(BUILD_DIR)

clean:
	@$(MSG_BEG)\033[0m\033[0;34mRecursive removing \033[1mbin\033[0m\033[0;34m directory:\033[0m\n"; fi
	$(LOG)rm -rf bin
	$(MSG_BEG)\033[0m\033[0;34mRemoving \033[1mtests\033[0m\033[0;34m symlink:\033[0m\n"; fi
	$(LOG)rm -f $(OBJECTS_TST) $(TEST_OBJ)
	$(MSG_BEG)\033[0m\033[0;34mRecursive removing \033[1mbuild\033[0m\033[0;34m directory:\033[0m\n"; fi
	$(LOG)rm -rf build

rebuild: clean all