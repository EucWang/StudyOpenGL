# StudyOpenGL

## study opengl

### learn opengl with https://learnopengl-cn.github.io

### study environment:
- win10
- VisualStudio 2019

### use package: 
- glfw, glad, glm, assimp

### basic set:
1. downlaod package: 
- downlaod glfw from  https://www.glfw.org/download.html
- select Windows -> pre-compiled binaries -> 32-bit Windows binaries

2. downlaod glad from https://glad.dav1d.de/
- at this web page select 
	- gl  -> Version 3.3
	- Profile -> Core
	- checked Options - Generate a loader
- then generate 
- then downlaod it


3. set visual studio:
- Project Configuration :
	- Configuration Properties
		- VC++ Directories : 
			- GEneral:
				- include directories: 
					- add glfw's include directory
					- add glad's include directory
				- library directories:
					- add glfw3's lib directory
		- Linker:
			- Input:
				- Additional Dependencies
					- add glfw3.lib;opengl32.lib

4. copy:
- copy glad.c to Project's directory


5. run:

- select [Release] [x86], other type has problem 