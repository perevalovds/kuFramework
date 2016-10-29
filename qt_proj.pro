#----- libs -----------------------------
INCLUDEPATH += ./libs

#--------- tinyxml ----------------------
INCLUDEPATH += ./libs/tinyxml

HEADERS += tinyxml.h
SOURCES += tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp



HEADERS += \
        float2.h	\
        float3.h	\
        int2.h	    \
        kuArgs.h	\
        kuConstants.h	\
        kuImages_qt.h	\
        kuIniSettings.h	\
        kuLog.h		\
        kuMain.h	\
        kuMath.h	\
        kuPoint.h	\
        kuRaster.h	\
        kuRasterProc.h	\
        kuRect.h	\
		kuSlowBit.h   \
        kuUtils.h	\
        kuValueMap.h	\
        kuXmlSettings.h


SOURCES += \
        float2.cpp	\
        kuArgs.cpp	\
        kuImages_qt.cpp	\
        kuIniSettings.cpp	\
        kuLog.cpp	\
        kuMath.cpp	\
        kuPoint.cpp	\
        kuRaster.cpp	\
        kuRasterProc.cpp	\
        kuRect.cpp	\
		kuSlowBit.cpp   \
        kuUtils.cpp	\
        kuValueMap.cpp	\
        kuXmlSettings.cpp
	



