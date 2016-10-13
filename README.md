# surveillance-video-system-plugin
add plugin for surveillance video system

directory organization:
main source file *.h *.cpp test_*.cpp will under $root/src directory
I will use cmake foreach to build library and test for each class under $root/src
other source file *.cpp *.h will under $root/test or $root
use pkgconfig to set the library and include directory, $root/pkgconfig/SVS-plugin.pc will do those work

IDE: QtCreator
to support auto-complete under QtCreator and call by other project, I need install the library by using cmake install.

usage:
ReDetection.cpp ReDetection.h test_ReDetection.cpp: ReDetection image
