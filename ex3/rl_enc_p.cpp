#include "rl_enc_p.h"

#define WRITE true
#define READ false

void rl_enc_p::process() {

    int         count;
    int         k;
    int         value;

    // reset cycle
    ready.write(false);
    wait();

    while (1) {
        // read DC value
        value = read_val();
        // write DC value
        write_val(value);

        count = 0;

        for( k = 1 ; k < 64 ; k++ ) {
            value = read_val();
            if ( value == 0 ) {
                count++;
            }
            else {
                write_val(count);
                count = 0;
                ready.write(false);
                wait();
                write_val(value);
            }
        }
        ready.write(false);
        wait();
        write_val(63);
    }
}

int rl_enc_p::read_val() {
    int value;
    ready.write(false);
    value = input.read();
    wait();
    return value;
}

void rl_enc_p::write_val(int value) {
    while (!ask.read()) wait();
    output.write(value);
    ready.write(true);
    wait();
}
