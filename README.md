avr-repl
--------

From time to time, one wants to quickly try electronics stuff (e.g.
"bitbang $custom_protocol to see how the device responds"). Usually, one
ends up with a hackish one-shot projects for those. However, this is
tedious:

 - You'll need some boilerplate (Makefiles, uC initialization) to
   bootstrap everything
 - When exploring a problem, you often want to try variations, i.e.
   "send byte 0x2a instead of 0x17". Being able to send commands to the
   firmware instead of recompiling and reflashing all the time makes
   experimentation easier.

For 3.3V level peripherals, https://github.com/florolf/usb-framework
works fine for that purpose. Sometimes though, you'll want to interact
with 5V stuff and level shifting requires extra parts.

This is where this project comes in: Get some cheap china AVR board with an
USB-serial converter on it, use this framework to hack up your stuff and get a
REPL to interact with it.
