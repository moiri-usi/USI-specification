SOURCES = main.cpp rl_dec_p.cpp rl_enc_p.cpp $(wildcard ../modules/*.cpp) ../add2systemc/my_fxtype_params.cpp
OBJS := $(SOURCES:.cpp=.o)
G++ = g++
SYSTEMC_DIR = /opt/systemc-2.3.0
I_DIR = -I ./ -I $(SYSTEMC_DIR)/include -I ../modules -I ../add2systemc
L_DIR = -L $(SYSTEMC_DIR)/lib-linux64
LIB = -lsystemc
test: $(OBJS)
	$(G++) $(I_DIR) $(L_DIR) -o $@ $(OBJS) $(LIB)
.cpp.o:
	$(G++) $(I_DIR) -c -o $@ $<
clean:
	rm -f test
	rm -f *.o
	rm -f *.vcd
	rm -f *.pgm
