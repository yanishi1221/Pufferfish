/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/gaolooong/Downloads/FreeDiving/src/FreeDiving.ino"
void setup();
void loop();
float averageValue(float arr[], int size);
void rotateServo();
#line 1 "/Users/gaolooong/Downloads/FreeDiving/src/FreeDiving.ino"
#define USE_ARDUINO_INTERRUPTS true // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>  // Includes the PulseSensorPlayground Library.
// #include </lib/PulseSensor_Playground/src/PulseSensorPlayground.h> // Includes the PulseSensorPlayground Library.

// list
const int MAX_SIZE = 10;       // 最大存储数量
float initialValues[MAX_SIZE]; // 存储最近的10个值的数组
float currentValues[5];        // 存储最近的10个值的数组
int initialIndex = 0;          // 当前要存储值的位置
float an = 0;
int frameCount = 0;

// servo
Servo myservo; // create servo object to control a servo
int pos = 0;   // variable to store the servo position

//  Variables
const int PulseWire = 0;           // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;       // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;               // Determine which Signal to "count as a beat" and which to ignore.
PulseSensorPlayground pulseSensor; // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

void setup()
{

  Serial.begin(9600); // For Serial Monitor
  myservo.attach(D0); // attaches the servo on the D0 pin to the servo object
  Serial.println("HEYYYY!");
  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED); // auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin())
  {
    Serial.println("We created a pulseSensor Object !"); // This prints one time at Arduino power-up,  or on Arduino reset.
  }

  for (int i = 0; i < MAX_SIZE; i++)
  {
    initialValues[i] = 0; // 初始化数组
  }
  for (int i = 0; i < 5; i++)
  {
    currentValues[i] = 0; // 初始化数组
  }
}

void loop()
{
  Serial.println("HEYYYY!");
  int myBPM = 0;
  if (pulseSensor.sawStartOfBeat())
  {
    myBPM = pulseSensor.getBeatsPerMinute();      // Constantly test to see if "a beat happened".
    Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
    Serial.println("BPM: ");                      // Print phrase "BPM: "
    Serial.println(myBPM);                        // Print the value inside of myBPM.
  }
  // 记录前10帧的BPM数值
  if (frameCount < 10)
  {
    if (myBPM != 0)
    {
      initialValues[initialIndex] = myBPM; // 将当前值存储在数组中
      frameCount++;
      initialIndex++;
    }
  }
  else if (frameCount == 10)
  {
    an = averageValue(initialValues, 10);
    for (int i = 0; i < 5; i++)
    {
      currentValues[i] = initialValues[i + 5];
    }
  }
  else // 正常运行后了
  {
    for (int i = 0; i < 5; i++)
    {
      if (i == 4)
      {
        currentValues[i] = myBPM;
      }
      else
      {
        currentValues[i] = currentValues[i + 1];
      }
    }
    float current = averageValue(currentValues, 5);
    if (current < an - 20)
    {
      Serial.println("Dying!");
    }
  }
  delay(20); // considered best practice in a simple sketch.
}

float averageValue(float arr[], int size)
{
  float sum = 0;
  for (int i = 0; i < size; i++)
  {
    sum += arr[i];
  }
  return sum / size;
}

void rotateServo()
{
  for (pos = 0; pos < 180; pos += 1) // goes from 0 degrees to 180 degrees
  {                                  // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 1; pos -= 1) // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);          // waits 15ms for the servo to reach the position
  }
}
