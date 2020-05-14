# ofxPlayground

An OpenFrameworks addon that provides scene management and cross-platform support for a certain type of generative projects. 2D focused. Officially supports OSX and iOS for now, might work on other platforms.


### Concepts

ofxPlayground abstracts inputs like mouse and touch (and other future inputs such as people detected via webcam) into the unified concept of a Target. So a mouse click is a Target and each touch position is a Target. Thus code in scene files should only be concerned with how those targets are represented and leave handling platform specific details of acquiring Targets to the addon code.


### Usage

User scenes should be implemented as classes in a subfolder called 'scenes'. See example-simple for details. In simple projects the user should not need to worry about anything outside 'scenes' (just copy main.cpp, ofApp.cpp and ofApp.h from the examples). 


### Examples

example-simple: contains two scenes that draw a box and a circle respectively and react to mouse input. Switching between scenes is provided by ofxPlayground so you can press any key to go from one scene to another.


### Helper functions

ofxPlayground includes some functions for generating useful shapes and drawing shapes in interesting ways. They are all available in every subclassed ofxPlaygroundScene via 'this->' (e.g. this->drawScatter). Some of the functions available are:

    drawScatter
    drawConcentric
    drawParticleFlow
    generateSubidividedSquare
    generateSubidividedLine

(These need more documentation, with pictures.)



