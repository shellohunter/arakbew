import os
import ConfigParser

CONFIG = {}

def getValueByLabel(config, section, label, prefix=None):
    x = config.get(section, label)
    y = x.split()
    if prefix != None:
        for i in range(len(y)):
            if not (y[i].startswith("/") or y[i].startswith(".")):
                y[i] = os.path.join(prefix, y[i])
    return y


def loadConfig():
    defaultConfigFile = "tools/build.inf" 
    if not os.path.exists(defaultConfigFile):
        print("can not find %s"%(defaultConfigFile))
    else:
        print("reading %s"%(defaultConfigFile))

    fp = open(defaultConfigFile)
    config = ConfigParser.ConfigParser()
    config.readfp(fp)

    for section in config.sections():
        branch              = getValueByLabel(config, section, "branch")[0]
        if branch == ".":
            branch = os.getcwd()
            branch = branch[0:branch.find("BDP_Generic")]

        CONFIG["BRANCH"] = branch
        CONFIG["LIB_PATH"]  = getValueByLabel(config, section, "libdirs", branch)
        CONFIG["INC_PATH"]  = getValueByLabel(config, section, "include", branch)
        CONFIG["LIBRARY"]   = getValueByLabel(config, section, "library")

    #print(CONFIG)

    return CONFIG

def checkQtPath(qtpath):    
    temp = os.path.join(qtpath,"bin/qmake")
    if not os.path.exists(temp):
       os.system("tar xjvf "+os.path.join(qtpath,"qt-4.7.0-vfp-cotexA9-install.tar.bz2")+" -C "+qtpath)
    if not os.path.exists(temp):
        return None
    print("qt ready")
    return qtpath

    return 0




def PkgReConfig(qt4path):
    allfile = os.listdir(os.path.join(qt4path, "lib/pkgconfig"))
    for each in allfile:
        fullpath = os.path.join(qt4path, "lib/pkgconfig", each)
        if fullpath.endswith(".pc"):
            f = open(fullpath)
            data = f.read()
            f.close()
            data = data.replace("/proj/mtk40438/BDP/Customer_Golden/BDP_V301_7118_8580_Golden/BDP_Linux/oss/source/qt/build/../qt-4.7.0/../../../library/qt", qt4path)
            f = open(fullpath, "w")
            f.write(data)
            f.close()



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
