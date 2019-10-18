CC      = gcc
CFLAGS  = -O3 -ansi -std=c11 -c -Wall -Wconversion -fPIC
# Test flags do not contain -Wconversion, because some tests must use implicit conversion
CFLAGST = -O3 -ansi -std=c11 -Wall
SRC_DIR = src
BIN_DIR = bin
TST_DIR = test
BUILD_DIR = build
COMPSRC = $(SRC_DIR)/array_list.c $(SRC_DIR)/array_map.c $(SRC_DIR)/string.c
COMPOBJ = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(COMPSRC))
SOURCES = $(COMPSRC)
OBJECTS = $(COMPOBJ)
SOURCES_TST = $(TST_DIR)/tests/array_list.c $(TST_DIR)/tests/array_map.c $(TST_DIR)/tests/string.c 
OBJECTS_TST = $(patsubst $(TST_DIR)/%.c, $(TST_DIR)/%.o, $(SOURCES_TST))
TEST_EXE = ./test.tcl
LIB      = libccomponents

OS_WIN = Windows_NT

ifeq ($(OS), $(OS_WIN)) 
        MSG_BEG = @echo "[CComponents] 
        MSG_END = "
	TEST_EXE = tclsh test.tcl
else
        E_PUR = \033[0;35m
        E_BLU = \033[0;34m
        E_RED = \033[0;31m
        E_B   = \033[1m
        E_0   = \033[0m
        E_0BLU = $(E_0)$(E_BLU)
        MSG_BEG = @if [ ! $$(echo -n $(LOG) | wc -m) -ge 1 ]; then printf "$(E_PUR)$(E_B)[CComponents]$(E_0) 
        MSG_END = "; fi
endif

# Set to empty to disable debug and @ to enable. Works also for TST
LOG = 
TST = 

all: $(BIN_DIR) $(OBJECTS) test $(LIB)

notest: $(BIN_DIR) $(OBJECTS) $(LIB)

$(BIN_DIR):
	$(MSG_BEG)$(E_BLU)Creating $(E_B)bin$(E_0BLU) directory:$(E_0)\n$(MSG_END)
	$(LOG)mkdir $(BIN_DIR)

$(BIN_DIR)/%.o : $(SRC_DIR)/%.c
	$(MSG_BEG)$(E_BLU)Compiling $(E_B)$^$(E_0BLU) source file:$(E_0)\n$(MSG_END)
	$(LOG)$(CC) $(CFLAGS) $< -o $@

$(TST_DIR)/%.o : $(TST_DIR)/%.c
	$(MSG_BEG)$(E_BLU)Building test $(E_B)$^$(E_0BLU):$(E_0)\n$(MSG_END)
	$(LOG)$(CC) $(CFLAGST) $< -o $@ $(COMPOBJ)

test: $(TEST_OBJ) $(OBJECTS_TST) $(COMPOBJ)
	$(MSG_BEG)$(E_RED)IF YOU HAVE NOT A TCL INTERPRETER YOU CAN BUILD PROJECT WITH "notest" GOAL$(E_0)\n$(MSG_END)
	$(MSG_BEG)$(E_RED)For example: make notest $(E_0)\n$(MSG_END)
	@if [ ! $$(echo -n $(TST) | wc -m) -ge 1 ]; then \
		printf "$(E_BLU)Running $(E_B)tests$(E_0BLU):$(E_0)\n"; fi
	@if [ ! $$(echo -n $(TST) | wc -m) -ge 1 ]; then \
		cd test && $(TEST_EXE) -debug ; else \
		cd test && $(TEST_EXE) -nout  ; fi
	$(MSG_BEG)$(E_BLU)Deleting $(E_B)tests object files:$(E_0)\n$(MSG_END)
	$(LOG)rm -f $(OBJECTS_TST) $(TEST_OBJ) 

$(LIB): $(OBJECTS)
	$(MSG_BEG)$(E_BLU)Creating $(E_B)build$(E_0BLU) directory:$(E_0)\n$(MSG_END)
	$(LOG)mkdir $(BUILD_DIR)
	$(MSG_BEG)$(E_BLU)Building $(E_B)library$(E_0BLU):$(E_0)\n$(MSG_END)
	$(LOG)$(CC) -Wall -shared -o $(BUILD_DIR)/$(LIB).so $(OBJECTS)
	$(MSG_BEG)$(E_BLU)Copying $(E_B)library header$(E_0BLU):$(E_0)\n$(MSG_END)
	$(LOG)cp $(SRC_DIR)/ccomponents.h $(BUILD_DIR)
	$(LOG)cp $(SRC_DIR)/foreach.h $(BUILD_DIR)

clean:
	@$(MSG_BEG)$(E_BLU)Recursive removing $(E_B)bin$(E_0BLU) directory:$(E_0)\n$(MSG_END)
	$(LOG)rm -rf bin
	$(MSG_BEG)$(E_BLU)Removing $(E_B)tests$(E_0BLU) symlink:$(E_0)\n$(MSG_END)
	$(LOG)rm -f $(OBJECTS_TST) $(TEST_OBJ)
	$(MSG_BEG)$(E_BLU)Recursive removing $(E_B)build$(E_0BLU) directory:$(E_0)\n$(MSG_END)
	$(LOG)rm -rf build

rebuild: clean all
