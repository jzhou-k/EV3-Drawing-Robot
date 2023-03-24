# EV3-Drawing-Robot

![Alt Text](link)
![Alt Text](link)
Original image of our digital computations professor 
![Alt Text](link)
Dotted image from the robot 


### Why
Our group sucks at technical drawings, so we made a robot that would do it for us.

### What 
This drawing robot will take in a .txt file containing pixel values of an image, uses a rack and pinion system to traverse each row and a paper rolling mechanism to traverse each column and another motor to dot on the paper based on the txt file. 

### How
The robot uses a text file that is output by a python program to be able to print. The python program takes in anything that can be exported through the printer dialogue on the computer and converts it into a pixel-format image. After this, a text file with 136 values across and 176 values down is created. Each value is a 1 or 0 to represent whether a dot is present or not. The robot reads this text file value by value and places a dot at every corresponding 1 value to produce a precise image. 
