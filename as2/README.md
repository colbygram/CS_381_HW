# Assignment 2: 
Plane on runway

# Features Not Present
Couldn't get the skybox functioning in time. Tried to reach out but never got a response so wasn't able to figure out the issue.

# Assignment Question:
I am confused by what the question is specifically asking? It references the camera but it isn't the specific goal to move the camera. If it means specifically moving the plane across different world axes, then yes, it would simplify the movement of the plane in a 2D sense, but for the plane to become 3D and actively move in 3D, it would be more effective to calculate the plane's position according to its own local axes opposed to the world axes. If the goal is to allow the camera to follow the plane/player, then it would be simpler to allow the camera to move on the same axes as the plane so that the plane is always in view.

# Pre-Install
Due to errors with MSVC, it is recommended to install WSL (https://learn.microsoft.com/en-us/windows/wsl/install). Using Ubuntu, WSL's default distribution, the following commands will install the G++ compiler which will allow for the code to be compiled.
```
sudo apt-get update
sudo apt-get install g++
```
Reminder that these commands must be run in the WSL shell and so should the following commands. The WSL shell can be accessed by using the ```wsl``` command in and Windows terminal

# Init Submodules
Enter the ```as2``` directory
```cd as2```

Initialize the submodules needed
```
 git submodule init
 git submodule update --init --recursive
```

# Prior To Build
When trying to compile with cmake, both my pc and laptop required these two commands to be installed on WSL for it actually compile. If you already know you have these two libraries present, no need to install them.
```
 sudo apt-get install libx11-dev
 sudo apt-get install xorg-dev libglu1-mesa-dev
```

# Build
Make a build directory ```mkdir build```. Navigate to it using ```cd build```. After navigating to the build folder, the user needs to use the following commands to generate the cmake build and build the executable.
```
mkdir build
cd build
cmake ..
make
```

# Run Executable and Operation
Use ```./as2``` to run the executable previously built. WASD for plane movement, ARROW KEYS for camera movement, Q and E for ascending and descending
