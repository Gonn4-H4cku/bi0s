import time
import serial
ser=serial.Serial('COM7',115200,timeout=1)
ser.write(str('#' + '\n').encode('ascii'))
time.sleep(1)

ser.write(str('m' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())
time.sleep(1)

ser.write(str('5' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)
ser.write(str('4'+'\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)
ser.write(str('1' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)
ser.write(str('2' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())
    
time.sleep(1)
ser.write(str('1' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)
ser.write(str('2' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)
ser.write(str('1' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)
ser.write(str('W' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)
ser.write(str('W' + '\n').encode('ascii'))
for i in ser.readlines():
    print(i.strip())

time.sleep(1)
ser.write(str('(2)' + '\n').encode('ascii'))
time.sleep(20)
ser.write(str('q' + '\n').encode('ascii'))
x = ser.readlines(1000)

print(x[3])

ser.write(str('#' + '\n').encode('ascii'))
ser.close()