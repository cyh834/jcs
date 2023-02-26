#include "all.h"

VerilatedContext *contextp = NULL;
VerilatedVcdC *tfp = NULL;
Vtop *top = NULL;

void sim_init(){
	contextp = new VerilatedContext;
	tfp = new VerilatedVcdC;
	top = new Vtop;
#ifdef HAS_WAVE
	contextp->traceEverOn(true);
	top->trace(tfp,99);
	tfp->open("wave.vcd");
#endif
}

static void step_and_dump_wave(){
#ifdef HAS_CLOCK
	top->clock = 1;
#endif
	top->eval();
#ifdef HAS_WAVE
	contextp->timeInc(1);
	tfp->dump(contextp->time());
#endif

#ifdef HAS_CLOCK
	top->clock = 0;
	top->eval();
#endif
}

void sim_exit(){
	step_and_dump_wave();
	tfp->close();
}

#ifdef HAS_RESET
void reset(){
	top->reset = 1;
	step_and_dump_wave();
	top->reset = 0;
}
#endif

void execute(uint64_t n){ while(n--) step_and_dump_wave();}

void exec(){execute(-1);}

void init(int argc, char *argv[]){
	sim_init();
#ifdef HAS_RESET
	reset();
#endif
}

int main(int argc, char *argv[]){
	init(argc,argv);

 // 将a，b改为1/0
	top->a=0; 	top->b=0;  step_and_dump_wave(); printf("%d\n",top->c); assert(top->c == (top->a ^ top->b)); 
           	    top->b=1;  step_and_dump_wave(); printf("%d\n",top->c); assert(top->c == (top->a ^ top->b));
    top->a=1; 	top->b=0;  step_and_dump_wave(); printf("%d\n",top->c); assert(top->c == (top->a ^ top->b));
                top->b=1;  step_and_dump_wave(); printf("%d\n",top->c); assert(top->c == (top->a ^ top->b));

	sim_exit();
}

