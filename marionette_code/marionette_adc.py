import sys
import marionette_lib as ml
# Connect to the Device
if len(sys.argv) > 1:
  port = sys.argv[1]
else:
  port = "/dev/serial/by-id/usb-APDM_Marionette_520022004115830393238353-if00"

m = ml.Marionette(port)
try:
	while True:
		#prompt the user to type start/ stop or exit
		x = raw_input('Please follow the instructions and type:\n1-"start" or "t" to start streaming data\n2-"stop" or "p" to stop streaming data\n3-"exit" or "e" to exit the program\n')
		if x == 'start' or x == 't' or x == '1':
			# configure adc0 sample rate as 1000 sample per second
			m.adc.config(0,1000)
			# configure adc1 sample rate as 1000 sample per second
			m.adc.config(1,1000)
			# start streaming samples over mpipe
			m.adc.start(0)
			m.adc.start(1)
			# display a confirmation message to the user that the streaming has begun
			print ('streaming has begun!')
			while True:
				x = raw_input('Please type "stop" or "p" to stop streaming data\n')
				if x == 'stop' or x == 'p' or x == '2':
					# stop streaming samples over mpipe
					m.adc.stop(0)
					m.adc.stop(1)
					# display a confirmation message to the user that the streaming has been stopped		
					print ('streaming has been stopped!')
					break
				else:
					# the user pressed a wrong key
					print('please try again and follow the instructions \n')
		elif x == 'stop' or x == 'p' or x == '2':
			# stop streaming samples over mpipe
			m.adc.stop(0)
			m.adc.stop(1)
			# display a confirmation message to the user that the streaming has been stopped		
			print ('streaming has been stopped!')
		elif x == 'exit' or x == 'e' or x == '3':
			# stop streaming samples over mpipe		
			m.adc.stop(0)
			m.adc.stop(1)
			# display a confirmation message to the user that we are exiting the loop
			print ('exiting!')
			# exiting the program
			break
		else:
			# the user pressed a wrong key
			print('please try again and follow the instructions \n')
# "Ctrl C" to Interrupt and exit the loop
except KeyboardInterrupt:
	print("\nKey interrupt")
	# stop streaming samples over mpipe		
	m.adc.stop(0)
	m.adc.stop(1)
	# display a confirmation message to the user that we are exiting the loop
	print ('exiting!')
