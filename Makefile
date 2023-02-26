TOPNAME = top  #顶层模块名字

VSRC_DIR = ./vsrc
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")


VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build --cc -O3 --x-assign fast \
 --x-initial fast --noassert --exe --trace

CFLAGS += -std=c++14 
LDFLAGS += 

sim: $(VSRCS) $(CSRCS)
	rm -rf $(BUILD_DIR)/*  
	$(VERILATOR) $(VERILATOR_CFLAGS) \
	--top-module $(TOPNAME) $^ \
	--Mdir $(OBJ_DIR) \
	$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) 

run: sim
	$(OBJ_DIR)/V$(TOPNAME)

wave: run
	gtkwave build/wave.vcd


clean:
	rm -rf $(VSRC_DIR)

.PHONY: sim wave run clean