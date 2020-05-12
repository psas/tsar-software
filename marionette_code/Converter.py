# Python code to demonstrate 
# converting hexadecimal string to decimal 
# Using int() 
  
# read string
test_string = raw_input("Enter your number : ")

  
# using int() 
# converting hexadecimal string to decimal 
res = int(test_string, 16) 

# Caclulate the voltage
voltage = (res * 3.3) / 1024

# print result 
print("Voltage:" + str(voltage))
