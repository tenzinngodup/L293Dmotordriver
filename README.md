Description 
The goal of this project is to construct a gear motor which uses lab jack to track the moving light source using an electronic photo sensor and runs the motor via L293D motor driver chip.  The project has two main circuits, the photo sensor circuit and the gear motor circuit connected across a LabJack U3-LV. 
For the gear motor circuit, L293D is connected with Motor on pin 3, 6 and the voltage regulator is connected on pin 16.  The battery of 6 V is connected to pin 9 which powers the motor, voltage supply of 5V is connected across pin 16 power the chip.  Pin 1 is used to control the motor with Pulse Width Modulation to send in the desired frequency and duty cycle is used to control the speed of the motor. 
The photo sensor is connected across the 5V regulator and a (FIO –analog in) lab jack, which is used to control the motor. The high input voltage is considered a light (as the photo sensor has low resistance) and low input voltage is considered dark (as photo sensor has high resistance). 
The light sensor circuit is connected to the wheel of the gear motor wheel and tracks the light source, relaying the signal to LabJack which is controlled by our C-program. The C-program interprets the input voltage on (FIO –analog in) and initiates motor towards the direction of light sensor

The Required components 
1  Gearmotor  (2 ft long twin-wire leads, male connectors, Wheel for gearmotor,1  L293D Quad half-H motor driver IC chip , 2  100 ?F capacitor (25 V, 20%, radial, polarized)  2  0.1 ?F capacitor (50 V, 20%, monolithic) , 1  Sensor cable (2 ft long twin-wire leads, M F connectors),1  Photocell (90 mW, 5K ? min, 20M ? max dark), 1  Battery clip (9 V) 


Theory
The resistance of the photocell depends on the amount of light illuminating its surface. The photo sensor has a high resistance when there is light on it and has low resistance when there is low light on it. All parts are functional. In our case we are using sensor which has cell resistance Max (dark condition)
of 20M ? Cell resistance Min (bright condition) of 5K ? with power dissipation of 90nW. 
Design Approaches
·	The Gear Motor is circuit is built first. The circuit has two major components of the circuit around the chip and the LabJack connection. 
·	A 5V power source is connected across the PIN 16 to power the chip and saturate any overflow of voltages in the chip. DAC0 is connected to PIN 16 in our circuit.
·	In our circuit PIN 1 is the 1,2EN which controlled the state of PIN 2 and PIN 7. High voltage signal of 1 enabled and 0 disabled the other two pins. 1,2EN   across FIO4 which uses PWM to control the frequency of the incoming signal to PIN 2 and PIN 7. The equation for the frequency is 
 bits            
FPT  is the frequency
TCB is the timer clock base
TCD is the timer clock divisor
Bits is the desired bit number                                                                                   
Where each components in declared in our C- program. We wanted the frequency of 15.295 Hz and thus our equation became
                                                                                                
·	The duty cycle is then used to control the speed of the motor. A 100% of duty cycle runs the motor at full speed as less as the percentage is decreased. The equation for the duty cycle is 
 Duty = (tw / tp) * 100%    where tp = 1 / f
·	PIN 2 and PIN 7 controlled the direction of the motor. When one pin has a higher voltage than the other, motor runs towards that direction and vice versa. PIN 2 is connected to FIO2 and PIN 7 is connected to FIO3 in our circuit. Low voltage of 1 PIN 2 and high voltage of 0 PIN 7 is a clockwise rotation and high voltage of 1 PIN 7 and low voltage of 0 PIN 7 is an anti- clockwise rotation. Equal voltage on both pin meant that motor will stop. 
·	The motor are connected on PIN 3 and PIN 6. 
·	The photo sensor is connected to DAC0 receiving a supply voltage of 5V which is then connected through FIO1 to a 10K resister and is grounded. A high voltage supply on FIO1 (low resistance on photo sensor) meant that it’s a bright condition and low voltage supply on FIO1 (high resistance on photo sensor) meant that it’s a dark condition. The threshold frequency for our bright condition was between (2.4 V to 1.8V) and the rest is considered as a dark condition (usually room light). 
Each of the major connection is attached with a capacitor which reduced the noise from the circuit