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

    SC_HAS_PROCESS(rl_enc_p);

    rl_enc_p(sc_module_name name):
        sc_module(name) {
            SC_THREAD(process);
            sensitive << clk.pos();
        }

    void process();
    int read_val();
    void write_val(int value);
};

#endif
