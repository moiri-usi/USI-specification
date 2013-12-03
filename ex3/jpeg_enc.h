/*  jpeg_enc.h */ 
#include        "r2b.h"
#include        "dct.h"
#include        "quantize.h"
#include        "zz_enc.h"
#include        "rl_enc_p.h"


SC_MODULE(jpeg_enc) {

    /* in and output */
    my_fifo_in<int>   input;
    my_fifo_in<int>   parameters;
    sc_out<int>       output;
    sc_in<bool>       ask;
    sc_in<bool>       clk;
    sc_out<bool>      ready;

    /* internal FIFOs */
    fifo_stat<int>        r2b_out;
    fifo_stat<float>      dct_out;
    fifo_stat<int>        quant_out;
    fifo_stat<int>        zz_enc_out;

    /* internal modules */
    r2b r2b_1;
    dct dct_1;
    quantize quant_1;
    zz_enc zz_enc_1;
    rl_enc_p rl_enc_1;

    jpeg_enc(sc_module_name _name, int* _quantization, int _maxwidth):
        sc_module(_name),
        r2b_out("r2b_out",1),
        dct_out("dct_out",1),
        quant_out("quant_out",1),
        zz_enc_out("zz_enc_out",1),
        r2b_1("r2b_1",_maxwidth),
        dct_1("dct_1"),
        quant_1("quant_1",_quantization),
        zz_enc_1("zz_enc_1"),
        rl_enc_1("rl_enc_1")
    {

        r2b_1.input(input);
        r2b_1.parameters(parameters);
        r2b_1.output(r2b_out);

        dct_1.input(r2b_out);
        dct_1.output(dct_out);

        quant_1.input(dct_out);
        quant_1.output(quant_out);

        zz_enc_1.input(quant_out);
        zz_enc_1.output(zz_enc_out);

        rl_enc_1.input(zz_enc_out);
        rl_enc_1.ask(ask);
        rl_enc_1.clk(clk);
        rl_enc_1.ready(ready);
        rl_enc_1.output(output);

    }

    ~jpeg_enc() {
    }

};
