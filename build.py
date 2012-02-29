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

    os.system("/usr/local/python2.7/bin/python tools/scons-2.1.0/bin/scons -j 4 --site-dir tools/scons-qt")

