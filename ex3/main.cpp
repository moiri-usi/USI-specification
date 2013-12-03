// toplevel function
// author: Marc Engels
#define      SC_INCLUDE_FX
#include    <systemc.h>
#include    "fifo_stat.h"
#include    "my_fxtype_params.h"
#include    "fix_stat.h"

#include    "test.h"
#include    "df_fork.h"
#include    "snk.h"
#include    "src.h"

#include    "jpeg_dec.h"
#include    "jpeg_enc.h"

#include    "FF2P.h"
#include    "P2FF.h"

#define MAXWIDTH 1024
#define MAXWIDTH8 ((MAXWIDTH +7)/8 * 8)

int sc_main (int argc , char *argv[]) {

/*
    int quantization[64] = { 8,  6,  6,  7,  6,  5,  8,  7,
                                 7,  7,  9,  9,  8, 10, 12, 20,
                                13, 12, 11, 11, 12, 25, 18, 19,
                                15, 20, 29, 26, 31, 30, 29, 26,
                                28, 28, 32, 36, 46, 39, 32, 34,
                                44, 35, 28, 28, 40, 55, 41, 44,
                                48, 49, 52, 52, 52, 31, 39, 57,
                                61, 56, 50, 60, 46, 51, 52, 50 };
*/

    int quantization[64] = { 16, 11, 10, 16, 24, 40, 51, 61,
                                 12, 12, 14, 19, 26, 58, 60, 55,
                                 14, 13, 16, 24, 40, 57, 69, 56,
                                 14, 17, 22, 29, 51, 87, 80, 62,
                                 18, 22, 37, 56, 68,109,103, 77,
                                 24, 35, 55, 64, 81,104,113, 92,
                                 49, 64, 78, 87,103,121,120,101,
                                 72, 92, 95, 98,112,100,103, 99};

// definition of default files
    const char* inputfile       = "datain.pgm";
    const char* outputfile      = "dataout.pgm";
    const char* typefile        = "types.txt";

//  definition of FIFO queues
    fifo_stat<int>  stimulus("stimulus",1);
    fifo_stat<int>  parameters("parameters",3);
    fifo_stat<int>  stimulus_dup1("stimulus_dup1",1);
    fifo_stat<int>  stimulus_dup2("stimulus_dup2",MAXWIDTH8*8+64+64+64+64+64);
    fifo_stat<int>  parameters_dup1("parameters_dup1",3);
    fifo_stat<int>  parameters_dup2("parameters_dup2",3);
    fifo_stat<int>  parameters_dup3("parameters_dup3",3);
    fifo_stat<int>  jpeg_enc_out("jpeg_enc_out",1);
    fifo_stat<int>  jpeg_enc_out_ff("jpeg_enc_out_ff",1);
    fifo_stat<int>  result("result",1);
    fifo_stat<int>  result_dup1("result_dup1",1);
    fifo_stat<int>  result_dup2("result_dup2",1);
    sc_clock clk("clock1", 20, SC_NS);
    sc_signal<bool> s_ask, s_ready;
    sc_signal<int> s_p_data;


// processing of command-line arguments
    bool    detected;
    for(int i=3; i<=argc; i+=2) {
        cout << argv[i-2] << " " << argv[i-1] << endl;
        detected = 0;
        if (strcmp(argv[i-2],"-i")==0) {
            inputfile = argv[i-1];
            detected = 1;
        }
        if (strcmp(argv[i-2],"-o")==0) {
            outputfile = argv[i-1];
            detected = 1;
        }
        if (strcmp(argv[i-2],"-t")==0) {
            typefile = argv[i-1];
            detected = 1;
        }
        if (detected == 0) {
            cout << "option " << argv[i-2] << " not known " << endl;
        }
    }

//  definition of modules

    src src1("src1", inputfile, MAXWIDTH);
    src1.output(stimulus);
    src1.parameters(parameters);

    df_fork<int,2> fork1("fork1");
    fork1.in(stimulus);
    fork1.out[0](stimulus_dup1);
    fork1.out[1](stimulus_dup2);

    df_fork<int,3> fork_param("fork_param");
    fork_param.in(parameters);
    fork_param.out[0](parameters_dup1);
    fork_param.out[1](parameters_dup2);
    fork_param.out[2](parameters_dup3);

    jpeg_enc jpeg_enc_1("jpeg_enc_1", quantization, MAXWIDTH);
    jpeg_enc_1.input(stimulus_dup1);
    jpeg_enc_1.parameters(parameters_dup1);
    jpeg_enc_1.output(s_p_data);
    jpeg_enc_1.clk(clk);
    jpeg_enc_1.ask(s_ask);
    jpeg_enc_1.ready(s_ready);

//    FF2P<int> ff2p1("ff2p1");
//    ff2p1.clk(clk);
//    ff2p1.input(jpeg_enc_out);
//    ff2p1.ask(s_ask);
//    ff2p1.ready(s_ready);
//    ff2p1.output(s_p_data);

//    P2FF<int> p2ff1("p2ff1");
//    p2ff1.clk(clk);
//    p2ff1.input(s_p_data);
//    p2ff1.ask(s_ask);
//    p2ff1.ready(s_ready);
//    p2ff1.output(jpeg_enc_out_ff);

    jpeg_dec jpeg_dec_1("jpeg_dec_1", quantization, MAXWIDTH, typefile);
    jpeg_dec_1.input(s_p_data);
    jpeg_dec_1.ask(s_ask);
    jpeg_dec_1.ready(s_ready);
    jpeg_dec_1.clk(clk);
    jpeg_dec_1.parameters(parameters_dup2);
    jpeg_dec_1.output(result);

    df_fork<int,2> fork2("fork2");
    fork2.in(result);
    fork2.out[0](result_dup1);
    fork2.out[1](result_dup2);

    snk snk1("snk1", outputfile);
    snk1.input(result_dup1);
    snk1.parameters(parameters_dup3);

    test test1("test1");
    test1.reference(stimulus_dup2);
    test1.data(result_dup2);

    sc_start();

    return 0;
}
