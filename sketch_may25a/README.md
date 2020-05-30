# sketch

Quick hack to wire up a 3 second timer on my front door switch.

Static IP in there for my internal private network (lol).

## HTTP GET /timer

Using a non-blocking timer loop which holds a pin on HIGH for 3 seconds.

## ctl_pins method

Wrapper to allow multiple I/O pins being switched at the same time.
