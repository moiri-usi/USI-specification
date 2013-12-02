/*  jpeg_dec.h */ 
#ifndef _JPEG_DEC
#define _JPEG_DEC
#include <systemc.h>
#include "add2systemc.h"
#include "b2r.h"
#include "idct.h"
#include "normalize.h"
#include "zz_dec.h"
#include "rl_dec.h"

SC_MODULE(jpeg_dec) {

    my_fifo_in<int>   input;
    my_fifo_in<int>   parameters;
    my_fifo_out<int>  output;
    sc_fifo<int> s_rl_dec, s_idct, s_zz_dec;
    sc_fifo<float> s_normalize;
    rl_dec rl_dec1;
    zz_dec zz_dec1;
    normalize normalize1;
    idct idct1;
    b2r b2r1;

    jpeg_dec(sc_module_name name, int _maxwidth, int* _quantization):
                sc_module(name),
                rl_dec1("dec1"),
                zz_dec1("zz_dec1"),
                normalize1("normalize1", _quantization),
                idct1("idct1"),
                b2r1("b2r1", _maxwidth) {
        rl_dec1.input(input);
        rl_dec1.output(s_rl_dec);

        zz_dec1.input(s_rl_dec);
        zz_dec1.output(s_zz_dec);

        normalize1.input(s_zz_dec);
        normalize1.output(s_normalize);

        idct1.input(s_normalize);
        idct1.output(s_idct);

        b2r1.input(s_idct);
        b2r1.parameters(parameters);
        b2r1.output(output);
        }
};

#endif
