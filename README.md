## What is this project?
This project consists in the realization of a race of speed and "intelligence" carried out only by an algorithm. Indeed, an algorithm written in C will communicate with the server to perform different checkpoints as quickly as possible. Beware these checkpoints are of a variable size. And each box of this checkpoint will have a value. <br><br>
<b>The goal of the game? </b> Arrive at the last checkpoint with the fewest points possible. A move is equal to 1 point and the boxes can give more or less points. At each turn the algorithm can accelerate or slow down by only 1 (Example: if its speed is 5 on the next turn it will be 4 5 or 6 only).

## Demo
![Preview](https://github.com/ThomasCorcoral/Projet_Racer_C/blob/master/racer.png)

## How was it done?

This project was completed in C. <br>
The viewer, the server and the three levels (race-dumb, race-mid, race-hi) were provided by M. Julien Bernard.

## Installation Guide

#### Step 1
Clone the GitHub project and go to the folder

##### Step 2
Compile racer.c (gcc -Wall -std=c99 -g -o racer racer.c)

##### Step 3
Open b>2/b> terminals

#### Step 4
In the first terminal, launch the viewer (./race-viewer)

#### Step 5
In the second terminal run the server with the participants desired>
For example, with only 1 participant: . /race-server . /racebr>
With all participants: . /race-server . /racer . /race-dumb . /race-mid . /race-hibr>

##### Step 6
To display the viewer again, you will need to restart it in the first terminal (./race-viewer)
