#pragma once

#include <stdint.h>

#define set_bit(byte, index, value) (byte) = (((byte) & ~(1 << (index))) | ((value) << (index)))
#define get_bit(byte, index) (!!((byte) & (1 << (index))))
#define toggle_bit(byte, index) (byte) ^= 1 << (index)
