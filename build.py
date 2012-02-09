#!/usr/local/bin/python

import os
import sys

if sys.platform=="win32":
    os.system("scons -Q --site-dir tools/scons-qt")
else:
    a=os.getcwd()
    a=a[:a.find("BDP_Linux")]
    os.putenv("Custom_QT4DIR", os.path.join(a,"BDP_Linux/oss/library/qt"))
    os.putenv("PKG_CONFIG_PATH", os.path.join(a, "BDP_Linux/oss/library/qt/lib/pkgconfig"))
    os.putenv("LD_LIBRARY_PATH", "/usr/local/python2.7/lib/")

    #os.system("export Custom_QT4DIR='/proj/mtk70628/WS_MTK70628_TEMP/BDP/Customer_Philips/BDP_V301_7118_8580_Golden_7675/BDP_Linux/oss/library/qt'")
    #os.system("export PKG_CONFIG_PATH=$Custom_QT4DIR'lib/pkgconfig'")
    #os.system("export LD_LIBRARY_PATH='/usr/local/python2.7/lib/'")
    
    os.system("/usr/local/python2.7/bin/python tools/scons-2.1.0/bin/scons -Q --site-dir tools/scons-qt")

