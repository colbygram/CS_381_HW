# ROBOT RUN
2D platformer
![alt text](<Github/Screenshot 2024-05-04 220139.png>)
## Objective:
You are E-7, an advanced cube that is looking to escape a mysterious environment
![alt text](<Github/Screenshot 2024-05-04 220203.png>)

Avoid enemy cubes that will reset progress!
![alt text](<Github/Screenshot 2024-05-04 220155.png>)

Find the exit and you win!!!
![alt text](<Github/Screenshot 2024-05-04 220221.png>)

## Controls
Use A and D to go left and right respectively. Use space to jump
## Criteria
Description of how criteria was met is in the ```Criteria.txt``` in ```as9``` folder
# Setup
## Pre-Install
Due to errors with MSVC, it is recommended to install WSL (https://learn.microsoft.com/en-us/windows/wsl/install). Using Ubuntu, WSL's default distribution, the following commands will install the G++ compiler which will allow for the code to be compiled.
```
sudo apt-get update
sudo apt-get install g++
```
Reminder that these commands must be run in the WSL shell and so should the following commands. The WSL shell can be accessed by using the ```wsl``` command in and Windows terminal
## Init Submodules
Enter the ```as9``` directory
```cd as9```

Initialize the submodules needed
```
 git submodule init
 git submodule update --init --recursive
```
## Prior To Build
When trying to compile with cmake, both my pc and laptop required these two commands to be installed on WSL for it actually compile. If you already know you have these two libraries present, no need to install them.
```
 sudo apt-get install libx11-dev
 sudo apt-get install xorg-dev libglu1-mesa-dev
```
## Build
Make a build directory ```mkdir build```. Navigate to it using ```cd build```. After navigating to the build folder, the user needs to use the following commands to generate the cmake build and build the executable.
```
mkdir build && cd build
cmake ..
make
```
## Run Executable and Operation
Use ```./as9``` to run the executable previously built. Planes and Boats use W and S to accelerate/decelerate. Boats use A and D to turn. Planes use the ARROW KEYS to maneuver in air
