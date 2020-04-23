while(1):
	# read string
	test_string = input("Enter your number : ")
 
	# Caclulate the voltage
	voltage = (test_string * 3.3) / 4096

	# print result 
	print("Voltage:", voltage)

