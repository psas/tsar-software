import sys
sys.path.append("../library/python/")
import marionette_lib as ml

if len(sys.argv) > 1:
  port = sys.argv[1]
else:
  port = "/dev/serial/by-id/usb-APDM_Marionette_520022004115830393238353-if00"

m = ml.Marionette(port)

# many gpio commands require a port (A-I)  and pin (0-15) on the stm32
# Note: in the future this will change to be a single string parameter


# make sure pins are at a known default configuration
# for most pins this is input floating
m.gpio.reset()

# set pin PG0 to output
m.gpio.config('g', 0, 'OUTPUT_PUSHPULL')

# set pin to HIGH
m.gpio.write('g', 0, 1)

# set pin to LOW
m.gpio.write('g', 0, 0)

# alternate way of setting HIGH
m.gpio.set('g',0)

# alternate way of setting LOW
m.gpio.clear('g', 0)

# set PG1 to input with pullup
m.gpio.config('g', 1, 'INPUT_PULLUP')

# read pin state
result = m.gpio.read('g', 1)
print result


