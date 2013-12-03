/*  rl_dec.h */ 
#ifndef _RL_DEC
#define _RL_DEC

#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(rl_dec_p) {
  
  my_fifo_in<int>   input;
  my_fifo_out<int>	output;

  SC_HAS_PROCESS(rl_dec_p);

  rl_dec_p(sc_module_name name):
		sc_module(name) {
			SC_THREAD(process);
		}
  
  void process();
};
#endif
