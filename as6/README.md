# Assignment 6: 
Entities and Components

# Features Not Present
Started on it way to late and wasn't able to finish. Can't figure out why the entity and components don't function because they had been on a previous build but are now broken.

# Pre-Install
Due to errors with MSVC, it is recommended to install WSL (https://learn.microsoft.com/en-us/windows/wsl/install). Using Ubuntu, WSL's default distribution, the following commands will install the G++ compiler which will allow for the code to be compiled.
```
sudo apt-get update
sudo apt-get install g++
```
Reminder that these commands must be run in the WSL shell and so should the following commands. The WSL shell can be accessed by using the ```wsl``` command in and Windows terminal

# Init Submodules
Enter the ```as6``` directory
```cd as6```

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
Use ```./as6``` to run the executable previously built.
