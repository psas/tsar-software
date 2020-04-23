import sys
sys.path.append("../library/python/")
import marionette_lib as ml

if len(sys.argv) > 1:
  port = sys.argv[1]
else:
  port = "/dev/serial/by-id/usb-APDM_Marionette_520022004115830393238353-if00"

m = ml.Marionette(port)

# make sure i2c pins are configured as i2c mode
m.i2c.config()

# write to i2c device
# first argument is the 7 bit address
# seconds argument is the number base to interpret bytes, reference strtoul
# following arguments are the bytes to send out, in this case the 4 bytes with values 0, 1, 2, 3
m.i2c.write(80, 0, 0, 1, 2, 3)

# read from i2c device
# first argument is the 7 bit address
# second argument is the number of bytes to read
result = m.i2c.read(80, 16)
print result

