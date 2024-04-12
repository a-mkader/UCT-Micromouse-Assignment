# EEE3088F Micromouse PCB-Assignment

A micromouse is, put very simply, a maze-solving robot. In this project, we will build a micromouse, focusing on the hardware of the robot, namely the power and sensing subsystems. <br /> <br />

## Motherboard Subsystem 
Fortunately, the motherboard subsystem has been designed for us. The motherboard houses the processor that will be the brain of the micromouse. <br />
![image](https://github.com/a-mkader/PCB-Assignment/assets/163734726/adeade09-d0c7-4884-b129-cedc638cf4bb)<br />
![image](https://github.com/a-mkader/PCB-Assignment/assets/163734726/4fcd885a-d044-4313-a8e8-c641b3d626aa)<br />

## Power Subsystem
This subsystem will be required to drive the two motors of the micromouse while being responsible for charging the battery that will supply the micromouse as well.
### Requirements:
- Provide a minimum of 400mA to drive two motors (200mA per motor)
* Charge the battery
  * Sense the battery's state of charge
+ Switch ON/OFF
- Be able to plug into the motherboard
* Be an appropriate size for the robot <br />

A more detailed description of these requirements, as well as the pinouts for this subsystem, are listed [here](EEE3088F_2024_project_description_v11.pdf) on page 7.

## Sensing Subsystem
This subsystem will act as the robot's eyes. It will have to determine whether or not there is an obstruction in the way of the robot, and then send this information to the processor to take necessary action.
### Requirements:
- Object detection 
 - Reliable way of communicating this information to the processor
* Power saving by means of switching
+ Be able to plug into the motherboard
- Needs to be an appropriate size for the robot
  
Once again, a more detailed description of what exactly this module needs to do as well as its pinouts, can be found [here](EEE3088F_2024_project_description_v11.pdf) on page 9.
