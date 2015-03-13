#pragma once

#include <avr/io.h>

#include "bit.h"

#define D0 D,0
#define D1 D,1
#define D2 D,2
#define D3 D,3
#define D4 D,4
#define D5 D,5
#define D6 D,6
#define D7 D,7
#define D8 B,0
#define D9 B,1
#define D10 B,2
#define D11 B,3
#define D12 B,4
#define D13 B,5

#define A0 C,0
#define A1 C,1
#define A2 C,2
#define A3 C,3
#define A4 C,4
#define A5 C,5

#define port_reg(combined) port_reg2(combined)
#define port_reg2(port, pin) (PORT ## port)

#define pin_reg(combined) pin_reg2(combined)
#define pin_reg2(port, pin) (PIN ## port)

#define ddr_reg(combined) ddr_reg2(combined)
#define ddr_reg2(port, pin) (DDR ## port)

#define pin(combined) pin2(combined)
#define pin2(port,pin) pin

#define port(combined) port2(combined)
#define port2(port,pin) port

#define make_output(combined) make_output2(combined)
#define make_output2(port, pin) set_bit(ddr_reg2(port, (pin)), (pin), 1)

#define make_input(combined) make_input2(combined)
#define make_input2(port, pin) set_bit(ddr_reg2(port, (pin)), (pin), 0)

#define set_output(combined, value) set_bit(port_reg2(combined), pin2(combined), (value))
#define set_pullup(combined, value) set_bit(port_reg2(combined), pin2(combined), (value))
#define get_output(combined) get_bit(port_reg2(combined), pin2(combined))
#define toggle_output(combined) toggle_bit(port_reg2(combined), pin2(combined))

#define get_input(combined) get_bit(pin_reg2(combined), pin2(combined))
