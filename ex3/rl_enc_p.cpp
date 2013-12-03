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
        communicate(&value, READ);
        // write DC value
        communicate(&value, WRITE);

        count = 0;

        for( k = 1 ; k < 64 ; k++ ) {
            communicate(&value, READ);
            if ( value == 0 ) {
                count++;
            }
            else {
                communicate(&count, WRITE);
                count = 0;
                communicate(&value, WRITE);
            }
        }
        value = 63;
        communicate(&value, WRITE);
    }
}

void rl_enc_p::communicate(int* value, bool action) {
    if (state == READINPUT) {
        if (action == WRITE) {
            read_val();
            wait();
            write_val(*value);
        }
        else {
            *value = read_val();
        }
    } 
    else if (state == WRITEOUTPUT) {
        if (action == READ) {
            write_val(0);
            *value = read_val();
            wait();
        }
        else {
            write_val(*value);
        }
    };
}

int rl_enc_p::read_val() {
    int value;
    ready.write(false);
    value = input.read();
    state = WRITEOUTPUT;
    cout << "READ: " << value << endl;
    return value;
}

void rl_enc_p::write_val(int value) {
    while (!ask.read()) {
        ready.write(false);
        cout << "WRITING..." << endl;
        wait();
    }
    output.write(value);
    ready.write(true);
    state = READINPUT;
    cout << "WRITE: " << value << endl;
    wait();
}
