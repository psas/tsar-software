import sys
sys.path.append("../library/python/")
import marionette_lib as ml


import sys
#========================================================#
print "This is the name of the script: ", sys.argv[0]
print "Number of arguments: ", len(sys.argv)
print "The arguments are: " , str(sys.argv)
#========================================================#
if len(sys.argv) > 1:
  port = sys.argv[1]
else:
  port = "/dev/serial/by-id/usb-APDM_Marionette_520022004115830393238353-if00"

m = ml.Marionette(port)

# config command is needed to set sample rate for streaming mode
# single sample mode it is not needed and the defaults are fine

# configure adc2 sample rate
m.adc.config(1, 100)

# start streaming samples over mpipe
m.adc.start(1)

# stop streaming samples over mpipe
m.adc.stop(1)

# read a single sample
results = m.adc.single(1)
print results

