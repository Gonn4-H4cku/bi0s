# Blink an LED
```cpp
void setup()
{
  pinMode(12, OUTPUT);
}

void loop()
{
  digitalWrite(12, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(12, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
}
```
# Control an led with button
```cpp
void setup()
{
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  if (digitalRead(12) == HIGH) 
  {
    digitalWrite(13, HIGH);
    delay(10); 
  }
  
  else 
  {
    digitalWrite(13 , LOW);
    delay(10);
  }
  
}
```

# control led with potentiometer
```cpp
int pot = 0;

int mapval = 0;

void setup()
{
  pinMode(A1, INPUT);
  pinMode(11, OUTPUT);
}

void loop()
{
  pot = analogRead(A1);
  mapval = map(pot, 0, 1023, 0, 255);
  analogWrite(11, mapval);
  delay(20); // Wait for 20 millisecond(s)
}
```
# tmp36
```cpp
int val1 = 0;
int val2 = 0;


void setup()
{
  pinMode(A0,INPUT);
  Serial.begin(9600);
}

void loop()
{
  val1 = analogRead(A0);
  val2 = map(val1 ,20,358,-40,125);
  Serial.println(val2);
  delay(20);
  
}
```
# Control a servo motor PWM
```cpp
#include <Servo.h>

Servo myserv;
  
void setup()
{
  
  myserv.attach(6);

}

void loop()
{
  int val1 = map(analogRead(A0),0,1023,0,180);
  myserv.write(val1);
  delay(10);
 
}
```

