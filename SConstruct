import sys
import os
from tools.build import *


#*********************************************
# Change code with care !!!!
#*********************************************

env=Environment()
env.Append(CPPPATH=['#source/include'])
env.Append(LIBPATH=['#source/library'])

if sys.platform=="win32":
    env['QT4DIR']   = "D:/Workspace/QT/4.6.2" # the path to your own QT folder
    env.Append(CCFLAGS="-DWINDOWS")
    env.Append(CPPPATH=['#source/include/win32'])
    env.Append(LIBPATH=['#source/library/win32'])
    env.Append(LIBS=['pthreadVC2', 'ws2_32'])
else:
    config = loadConfig()
    qtpath = checkQtPath(os.path.join(config["BRANCH"], "BDP_Linux/oss/library/qt"))
    if qtpath == None:
        print("Qt path error %s"%(qtpath))
        quit()
    PkgReConfig(qtpath)
    env['QT4DIR']   = qtpath
    env['ENV']['PKG_CONFIG_PATH']    = os.path.join(qtpath, "lib/pkgconfig")
    CROSS_COMPILER_PREFIX="/mtkoss/gnuarm/vfp_4.5.1_2.6.27_cortex-a9-rhel4/i686/bin/armv7a-mediatek451_001_vfp-linux-gnueabi-"
    env.Replace(CC=[CROSS_COMPILER_PREFIX+"gcc"])
    env.Replace(CXX=[CROSS_COMPILER_PREFIX+"g++"])
    env.Replace(AR=[CROSS_COMPILER_PREFIX+"ar"])
    env.Replace(AS=[CROSS_COMPILER_PREFIX+"as"])
    env.Replace(LINK=[CROSS_COMPILER_PREFIX+"g++"])
    env.Append(CPPPATH=config["INC_PATH"])
    env.Append(LIBPATH=config["LIB_PATH"])
    env.Append(LIBS=config["LIBRARY"])

env.Tool('qt4')
env.EnableQt4Modules(['QtGui','QtCore','QtNetwork'])

Export("env")
VariantDir("#.temp", 'source', duplicate=0)

build(env, "source", "#.temp")


