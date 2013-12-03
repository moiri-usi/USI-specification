/*  rl_enc.h */ 
#ifndef _RL_ENC
#define _RL_ENC 
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(rl_enc_p) {
    my_fifo_in<int> input;
    sc_out<int>     output;
    sc_in<bool>     ask;
    sc_out<bool>    ready;
    sc_in<bool>     clk;
    enum ctrl_state {READINPUT, WRITEOUTPUT};
    ctrl_state  state;

    SC_HAS_PROCESS(rl_enc_p);

    rl_enc_p(sc_module_name name):
        sc_module(name) {
            state = READINPUT;
            SC_THREAD(process);
            sensitive << clk.pos();
        }

    void process();
    void waiting();
};

#endif
