import serial
import time

ser = serial.Serial('COM3', 115200)
time.sleep(2)

while True:
    ser.write("\n".encode('utf-8'))
    time.sleep(0.5)
    ser.write("JUMP\n".encode('utf-8'))
    time.sleep(0.5)

    print(ser.read_all().decode('utf-8'))

ser.close()