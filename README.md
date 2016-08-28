==kuFramework==
This is porting of openFrameworks non-graphic modules for using in non-openFrameworks projects.

There are several modules, which are platform dependent:
*kuImages_of,
*kuImages_qt,
*and others.

If you need no image processing using kuRaster_..., just don't include them in your project.

To work with images, use one of these modules, corresponding to your platform:
you can save and load kuRaster_... objects to image files, and transform them 
to QImage (Qt), ofPixels (oF) classes and back
