import serial, time, ctypes, random, threading

# Constants
RECEIVE_PAYLOAD_LENGTH = 5 # The length in bytes of the actuator commands
RECEIVE_PACKET_LENGTH = 7 + RECEIVE_PAYLOAD_LENGTH # The length in bytes of the preamble, payload, and 
SEND_PAYLOAD_LENGTH = 6
SEND_PACKET_LENGTH = SEND_PAYLOAD_LENGTH + 10
MAXIMUM_LIFETIME_CHECKSUM_ERRORS = 25 

def combine(a, b):
	return a + (b << 8)

def checksum(message):
	result = 0
	for i in message:
		result += (i*i*i)%251

	return result

def read_packet(s):
	while s.read(1) != b'\n':
		pass

	# return b'\n' + s.read(SEND_PACKET_LENGTH-1)
	return Packet(b'\n' + s.read(SEND_PACKET_LENGTH-1))

class Packet:
	def __init__(self, packet):
		self._data = packet
		self.valid = packet[0] == 10 # Check for starting newline
		self.safest_next_failure_mode = packet[1]
		self.failure_mode = packet[2]
		self.failure_cause = packet[3]
		self.consecutive_checksum_error_count = packet[4]
		self.lifetime_checksum_error_count = packet[5]
		self.sensors = list(bytes(packet[6:-4]))
		self.checksum = packet[-4:].hex()

	def __hash__(self):
		return hash(self._data)

	def __str__(self):
		return f"""Packet(
	valid = {self.valid}
	safest_next_failure_mode = {self.safest_next_failure_mode}
	failure_mode = {self.failure_mode}
	failure_cause = {self.failure_cause}
	consecutive_checksum_error_count = {self.consecutive_checksum_error_count}
	lifetime_checksum_error_count = {self.lifetime_checksum_error_count}
	sensors = {self.sensors[:4] + [combine(self.sensors[4], self.sensors[5])]}
	checksum = {self.checksum}
)"""

s = serial.Serial("COM3", 9600, timeout=3)
print("Waiting for COM to initialize...", end="", flush=True)
time.sleep(2)
print("Pumping excess data:", s.read_all().hex())
print("Pumping excess data:", s.read_all().hex())
print("Done.")

old_hash = 0
loop_count = 0

while True:
	print("Loop count:", loop_count // 10)
	data = bytes([ord("\n"), 255, 15]) + bytes([loop_count // 10, 0, 0, 0, 0])
	print(data.hex())
	data += bytes(ctypes.c_uint32(checksum(data)))# + (1 if random.randint(0, 4) == 1 else 0)))
	s.write(data)
	# print("Sending packet: " + data.hex())
	loop_count += 1

	packet = None
	while (s.in_waiting >= SEND_PACKET_LENGTH):
		packet = read_packet(s)
	# if packet != None:
		# print("Data:", packet._data.hex())
	print(packet)
	print("\n")

	if hash(packet) != old_hash:
		print(packet)
		old_hash = hash(packet)

	time.sleep(.4)
