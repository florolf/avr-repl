#pragma once

#include <avr/io.h>

#include "bit.h"

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
