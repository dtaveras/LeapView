LeapView
========

Paraview Simple Leap Motion Plugin

Entirely clean Paraview "View" no longer has unecessary code

This view has support to allow the user to control the 3D View with the leap Motion. 
The text in the View indicates whether the Leap Motion is activated or not and also gives 
the viewer some information about the 3D coordinates seen by the Leap Motion Device.
There are several buttons in order to control rotation of the object as well as rotation of the
camera.

----------------------------------------------
Build Instructions
----------------------------------------------

mkdir build;
cd build; ccmake ..; //You need to specify your paraview directory
cd ..; ./addExtraLink.sh; //Run the extralink script to add leapmotion dylib to link.txt
cd build; make; //Now you should be done and ready to load in paraview