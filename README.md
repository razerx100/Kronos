# Kronos
Game engine (Under development).

# Requirements
1: cmake 3.12+.\
2: make (For building on Linux).\
3: Visual Studio 2019 (For building on Windows).

# Instructions
First clone the repository. Then initialize the git submodules. Now follow either of the steps below.

### Windows
Run the Setup.bat to setup the cmake project. Visual Studio's solution file will be generated in the build directory. The project can then be opened as either a solution or a cmake project in Visual Studio.
	
### Linux
Run the Setup.bash to setup the cmake project. Then run the build.bash to build the project and run the generated executable on build/bin/Linux/{Debug or Release}/ from a terminal.\
Note: Currently no Window Implementation for Linux has been done. So, only setup the project for developement purpose; don't build on Linux.

