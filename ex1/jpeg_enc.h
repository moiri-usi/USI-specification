/*  jpeg_enc.h */ 
#ifndef _JPEG_ENC
#define _JPEG_ENC
#include <systemc.h>
#include "add2systemc.h"
#include "r2b.h"
#include "dct.h"
#include "quantize.h"
#include "zz_enc.h"
#include "rl_enc.h"

SC_MODULE(jpeg_enc) {

    my_fifo_in<int>   input;
    my_fifo_in<int>   parameters;
    my_fifo_out<int>  output;
    sc_fifo<int> s_r2b, s_quantize, s_zz_enc;
    sc_fifo<float> s_dct;
    r2b r2b1;
    dct dct1;
    quantize quantize1;
    zz_enc zz_enc1;
    rl_enc rl_enc1;

    jpeg_enc(sc_module_name name, int _maxwidth, int* _quantization):
            sc_module(name),
            r2b1("r2b1", _maxwidth),
            dct1("dct1"),
            quantize1("quantize1", _quantization),
            zz_enc1("zz_enc1"),
            rl_enc1("rl_enc1") {
        r2b1.input(input);
        r2b1.parameters(parameters);
        r2b1.output(s_r2b);

        dct1.input(s_r2b);
        dct1.output(s_dct);

        quantize1.input(s_dct);
        quantize1.output(s_quantize);

        zz_enc1.input(s_quantize);
        zz_enc1.output(s_zz_enc);

        rl_enc1.input(s_zz_enc);
        rl_enc1.output(output);
    }
};

#endif
