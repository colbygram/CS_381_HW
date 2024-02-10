# Assignment 1: 
Render models and textures into window

![Untitledvideo-MadewithClipchamp6-ezgif com-optimize](https://github.com/colbygram/CS_381_HW/assets/123613082/f7ac9198-ba37-4751-b03e-a7d7cf1729a9)

# Pre-Install
Due to errors with MSVC, it is recommended to install WSL (https://learn.microsoft.com/en-us/windows/wsl/install). Using Ubuntu, WSL's default distribution, the following commands will install the G++ compiler which will allow for the code to be compiled.
```
sudo apt-get update
sudo apt-get install g++
```
Reminder that these commands must be run in the WSL shell and so should the following commands. The WSL shell can be accessed by using the ```wsl``` command in and Windows terminal

# Init Submodules
Enter the ```as1``` directory
```cd as1```

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
Use ```./as1``` to run the executable previously built.
