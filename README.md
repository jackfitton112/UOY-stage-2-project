# Micro Mouse Project

> Note: This is still under development and may not fully work at the moment, there are serveral features that are not yet implemented.

> Currently `obsticle-avoidance` is under development and is not yet fully functional. all other files are tested and working.


The micro mouse is a small robot that is designed to solve a maze. It is equipped with an ESP 32 microcontroller and programmed using the Arduino/C programming language. The mouse uses a combination of sensors and algorithms to navigate through the maze and find the fastest path to the end.

The micro mouse uses a VL53L1X sensor to detect obstacles and determine its position in the maze. The sensor emits a laser beam and measures the time it takes for the beam to bounce back, allowing the mouse to detect objects and obstacles in its path.

In addition to the VL53L1X sensor, the micro mouse also uses encoders to measure the distance it travels and keep track of its orientation. Encoders are small devices that are attached to the wheels of the mouse, and they measure the rotation of the wheels to determine the distance the mouse has moved.

The mouse uses the information from the sensors and encoders to generate a map of the maze and plan the best path to the end. It then follows the plan by moving through the maze and making adjustments as needed based on the information it receives from the sensors.

The micro mouse may use a variety of algorithms to solve the maze, such as wall following or flood fill algorithms. These algorithms allow the mouse to explore the maze and find the shortest path to the end. Over time, the mouse may improve its ability to solve the maze by adjusting and refining its algorithms.

---
## Important Infomation:

#### Motor Pinout:

|  | Motor A | Motor B |
| :---: | :-----------: | :----: |
| PWM | 13 |12|
| Input A | 25 |2|
| Input B | 26 |4|
| Encoder A | 34 |32|
| Encoder B | 35 |33|


#### Sensor Pinout:

| Pins | Sensor A (VL53L1X) | Sensor B (VL53L0X) |
| :---: | :-----------: | :----: |
| SDA | 21 |21|
| SCL | 22 |22|
| XSHUT | N/A |N/A|
| GPIO1 / INT | N/A |N/A|

> Note: XSHTU and GPIO1 / INT are not used for now. they will be used in the future for both sensors.



#### notes:

there are going to be a few crutial functions, these will include:

a function to move the mouse forward
a function to turn left and right
a watchdog to make sure the mouse is moving straight 
a watchdog to make sure the mouse is not stuck / hit a wall

> this will probs require getting the int pin on the tof working as then if if is stuck it will send a signal to the microcontroller and the microcontroller will then stop the motors



