import serial
# xác định cổng đầu vào của USART để thực hiện truyền nhận
ser = serial.Serial("COM5", baudrate=9600)


def receiving():
    global last_received
    buffer_string = ''
    while True:
        buffer_string = buffer_string + str(ser.read(ser.inWaiting()))
        if '\n' in buffer_string:
            print('yes')
            lines = buffer_string.split('\n') # Guaranteed to have at least 2 entries
            last_received = lines[-2]
            #If the Arduino sends lots of empty lines, you'll lose the
            #last filled line, so you could make the above statement conditional
            #like so: if lines[-2]: last_received = lines[-2]
            buffer_string = lines[-1]
            print(buffer_string)
        
a = receiving()