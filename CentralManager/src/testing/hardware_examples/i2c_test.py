import time
import smbus

i2c_ch = 1

# TMP102 address on the I2C bus
i2c_address = 0x68

# Initialize I2C (SMBus)
bus = smbus.SMBus(i2c_ch)

out = bus.read_byte(i2c_address)
print("output:", out)
