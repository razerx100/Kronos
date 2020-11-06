# Kronos
Game engine(in the making).

# Requirements
1: cmake 3.12+
2: make (For building on Linux)

# Instructions
	Clone the repository. Initilize the git submodules.

	# Windows
	Run the Setup.bat to setup the cmake project. Visual Studio solution would be generated in build directory.
	Note: If you open the project as a cmake project in Visual studio and edit the CMakeLists.txt, it'll generate the project for ninja. If you want to open it as a solution later on, run the Setup.bat to generate the sln file.
	
	# Linux
	Run the Setup.bash to setup the cmake project. Then run the build.bash to build the project and run the generated executable on build/bin/Linux/{Debug or Release}/ from a terminal.

