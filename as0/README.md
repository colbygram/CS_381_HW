# Assignment 0: 
Resizable window with mutlicolor fading text

![Untitledvideo-MadewithClipchamp-ezgif com-video-to-gif-converter](https://github.com/colbygram/as0/assets/123613082/929f4fe0-4fc8-4e12-b813-33f628085b0e)

# Pre-Install
Due to errors with MSVC, it is recommended to install WSL (https://learn.microsoft.com/en-us/windows/wsl/install). Using Ubuntu, WSL's default distribution, the following commands will install the G++ compiler which will allow for the code to be compiled.
```
sudo apt-get update
sudo apt-get install g++
```
Reminder that these commands must be run in the WSL shell and so should the following commands. The WSL shell can be accessed by using the ```wsl``` command in and Windows terminal

# Init Submodules
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
Firstly, navigate to the as0 folder
```
cd as0
```
Then use the following commands to make the build folder and build the project
```
mkdir build
cd build
cmake ..
make
```

# Run Executable and Operation
Use ```./as0``` to run the executable previously built. Simply click and drag to resize the window.

# Bug
I had this bug on my PC. I don't know if it will show up one any other device, because I went through my processes on my laptop and it looked fine.
![Screenshot 2024-02-05 195050](https://github.com/colbygram/CS_381_HW/assets/123613082/bad4e1c2-4d91-408b-bef9-3db2c14e9ecb)
