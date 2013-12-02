// toplevel function
// author: Marc Engels
#include <systemc.h>    //-- the SystemC main header file
#include "src.h"    //-- include src definition
#include "snk.h"    //-- include snk definition
#include "jpeg_enc.h"
#include "jpeg_dec.h"

#define MAXWIDTH 1024
int quantization[] = {16, 11, 10, 16, 24, 40, 51, 61,
                        12, 12, 14, 19, 26, 58, 60, 55,
                        14, 13, 16, 24, 40, 57, 69, 56,
                        14, 17, 122, 29, 51, 87, 80, 61,
                        18, 22, 37, 56, 68, 109, 103, 77,
                        24, 35, 55, 64, 81, 104, 113, 92,
                        49, 64, 78, 87, 103, 121, 120, 101,
                        72, 92, 95, 98, 112, 100, 103, 99};

int sc_main (int argc , char *argv[]) {
  sc_fifo<int>   stimulus, parameter, parameters[3];
  fifo_stat<int>   s_jpec_encoded, stimulus_rl_dec;

  const char*   inputfile       = "datain.pgm";
  const char*   outputfile      = "dataout.pgm";


// processing of command-line arguments
    for(int i=3; i<=argc; i+=2) {
        cout << argv[i-2] << " " << argv[i-1] << endl;
        bool detected = 0;
        if (strcmp(argv[i-2],"-i")==0) {
            inputfile = argv[i-1];
            detected = 1;
        }
        if (strcmp(argv[i-2],"-o")==0) {
            outputfile = argv[i-1];
            detected = 1;
        }
        if (detected == 0) {
            cout << "option " << argv[i-2] << " not known " << endl;
        }
    }

// here comes your code
    src src1("src1", inputfile, MAXWIDTH);
    src1.output(stimulus);
    src1.parameters(parameter);

    df_fork<int, 3> df_fork1("df_fork1");
    df_fork1.in(parameter);
    df_fork1.out[0](parameters[0]);
    df_fork1.out[1](parameters[1]);
    df_fork1.out[2](parameters[2]);

    jpeg_enc jpec_enc1("jpec_enc1", MAXWIDTH, quantization);
    jpec_enc1.input(stimulus);
    jpec_enc1.parameters(parameters[0]);
    jpec_enc1.output(s_jpec_encoded);

    jpeg_dec jpec_dec1("jpec_dec1", MAXWIDTH, quantization);
    jpec_dec1.input(s_jpec_encoded);
    jpec_dec1.parameters(parameters[1]);
    jpec_dec1.output(stimulus_rl_dec);

    snk snk1("snk1", outputfile);
    snk1.input(stimulus_rl_dec);
    snk1.parameters(parameters[2]);

// start of simulation
  sc_start();
  return 0;
}
