import time 
import serial
from serial import Serial
ser  = serial.Serial(port = 'COM7',baudrate = 115200,timeout=1)

ser.write(str('#' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)

ser.write(str('m' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)

ser.write(str('4' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)

ser.write(str('\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)

ser.write(str('W' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)

ser.write(str('P' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)

ser.write(str('(2)' + '\n').encode('ascii'))
time.sleep(10)
ser.write(str('q' + '\n').encode('ascii'))

l = ser.readlines(100)
print(l[3:])

ser.write(str('#' + '\n').encode('ascii'))
ser.close()