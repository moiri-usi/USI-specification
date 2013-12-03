#include "rl_dec_p.h"

void rl_dec_p::process() {

    int     count;
    int     value;

    while (1) {
        ask.write(true);
        wait();
        // read and write DC value
        value = read_val();
        write_val(value);
        // process AC values
        count = 1;
        // read zero length value
        value = read_val();
        while (value != 63) {
            // process zl
            for ( ; value > 0 ; value-- ) {
                output.write(0);
                count++;
            }
            // read and process non-zero value
            ask.write(true);
            wait();
            value = read_val();
            write_val(value);
            count ++;
            // read zero length value
            value = read_val();
        }
        // process zero length of 64
        for ( ; count<64 ; ) {
            output.write(0);
            count++;
        }
    }
    return;
}

int rl_dec_p::read_val() {
    int value;
    while (!ready.read()) wait();
    value = input.read();
    ask.write(false);
    wait();
    return value;
}

void rl_dec_p::write_val(int value) {
    output.write(value);
    ask.write(true);
    wait();
}
