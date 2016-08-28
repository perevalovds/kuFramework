#kuFramework
kuFramework is cross-platform porting of openFrameworks non-graphic modules for using in non-openFrameworks projects.

It includes 
* working with ini-files 'kuIniSettings'
* working with xml-files 'kuXmlSettings'
* 2d and 3d vectors 'int2', 'int3', 'float2', 'float3'

To use kuFramework in your project, include its .h and .cpp files to your project, except kuImages_... files (see description below)
and add '#include "kuMain.h" to use it.


##Working with images
There are several modules, which are platform dependent:
* kuImages_of,
* kuImages_qt,
* and others.

If you need no image processing using kuRaster_..., just don't include them in your project.

To work with images, use one of these modules, corresponding to your platform:
you can save and load kuRaster_... objects to image files, and transform them 
to QImage (Qt), ofPixels (oF) classes and back


##Platforms
kuFramework is tested in Windows, OSX, Linux.
