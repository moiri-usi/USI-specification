/*  rl_dec.h */ 
#ifndef _RL_DEC
#define _RL_DEC

#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(rl_dec_p) {

    sc_in<int>          input;
    my_fifo_out<int>    output;
    sc_out<bool>        ask;
    sc_in<bool>         ready;
    sc_in<bool>         clk;

    SC_HAS_PROCESS(rl_dec_p);

    rl_dec_p(sc_module_name name):
        sc_module(name) {
            SC_THREAD(process);
            sensitive << clk.pos();
        }

    void process();
    int read_val();
    void write_val(int value);
};
#endif
