#include "rl_dec.h"

void rl_dec::process() {

    int count;
    int k;
    int value;

    while (1) {
        count = 0;
        value = 0;
        do {
            for (k = 0; k < value; k++) {
                output.write(0);
                count++;
            }
            // read and write DC value
            output.write(input.read());
            count++;
            value = input.read();
        }
        while (value != 63);

        for (k = 0; k < 64-count; k++) {
            output.write(0);
        }
    }
}
