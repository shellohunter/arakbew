import sys
import os
from tools.myhelper import *


#*********************************************
# Change code with care !!!!
#*********************************************



def build(env, sourcepath, objpath="#.temp"):
    if os.path.isfile(sourcepath):
        if sourcepath.endswith(".cpp") or sourcepath.endswith(".c"):
            print(sourcepath.replace("source", objpath))
            env.Object(sourcepath.replace("source", objpath))
    elif os.path.isdir(sourcepath):
        if os.path.exists(os.path.join(sourcepath,"SConscript")):
            print(os.path.join(sourcepath,"SConscript"))
            env.SConscript(os.path.join(sourcepath,"SConscript").replace("source", objpath))
        all = os.listdir(sourcepath)
        for each in all:
            subpath = os.path.join(sourcepath, each)
            build(env, subpath)


env=Environment()
env.Append(CPPPATH=['#source/include'])
env.Append(LIBPATH=['#source/library'])

if sys.platform=="win32":
    #env['QT4DIR']   = "D:/Workspace/QT/4.6.2" # the path to your own QT folder
    env['QT4DIR']   = "D:/Software/QT4.7.3" # the path to your own QT folder
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
    CROSS_COMPILER_PREFIX=config["TOOLCHAIN"]
    env.Replace(CC=[CROSS_COMPILER_PREFIX+"gcc"])
    env.Replace(CXX=[CROSS_COMPILER_PREFIX+"g++"])
    env.Replace(AR=[CROSS_COMPILER_PREFIX+"ar"])
    env.Replace(AS=[CROSS_COMPILER_PREFIX+"as"])
    env.Replace(LINK=[CROSS_COMPILER_PREFIX+"g++"])
    env.Append(CPPPATH=config["INC_PATH"])
    env.Append(LIBPATH=config["LIB_PATH"])
    env.Append(LIBS=config["LIBRARY"])
    env.Append(CCFLAGS="-O2 -Wall")


env.Tool('qt4')
env.EnableQt4Modules(['QtGui','QtCore','QtNetwork'])

Export("env")
VariantDir("#.temp", 'source', duplicate=0)

build(env, "source", "#.temp")


