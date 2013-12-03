#include "rl_enc_p.h"

#define WRITE true
#define READ false

void rl_enc_p::process() {

    int         count;
    int         k;
    int         value;

    // reset cycle
    ready.write(false);
    state = READINPUT;
    wait();

    while (1) {
        // read DC value
        ready.write(false);
        value = input.read();
        wait();
        // write DC value
        waiting();
        output.write(value);
        ready.write(true);
        wait();

        count = 0;

        for( k = 1 ; k < 64 ; k++ ) {
            ready.write(false);
            value = input.read();
            wait();
            if ( value == 0 ) {
                count++;
            }
            else {
                waiting();
                output.write(count);
                ready.write(true);
                wait();
                count = 0;
                ready.write(false);
                wait();
                waiting();
                output.write(value);
                ready.write(true);
                wait();
            }
        }
        ready.write(false);
        wait();
        waiting();
        output.write(63);
        ready.write(true);
        wait();
    }
}

void rl_enc_p::waiting() {
    while (!ask.read()) {
        wait();
    }
}
