-----------------------------------------------------------------------
How to build it on win32 system ?
-----------------------------------------------------------------------
1. First of all you should have the following components properly installed.
    a. Visual C++           (express edition will do.)
    b. QT 4.x for MSVC      (http://qt.nokia.com/)
    c. Python 2.7.x         (http://www.python.org/)
    d. Scons 2.0 or later.  (http://www.scons.org/)

2. Assume that your project is located at "X:\somepath\webkara\".
   Open "X:\somepath\webkara\Sconstrct" with an txt editor, find:
    if sys.platform=="win32":
        env['QT4DIR']   = "" # the path to your own QT folder
   Change the path to where your QT was installed.

3. Then enter DOS command line, "cd" to the project folder, excute:
   X:\somepath\webkara>scons

That's all.


-----------------------------------------------------------------------
How to build it on linux server ?
-----------------------------------------------------------------------
1. Make sure you have a 8580 workspace, then perform a full build.

2. The project is located at "BDP_Linux/mtk/source/webkara" by default.
   "cd" to that folder, and excute:
   someone$ sh build.sh

Thas't all.


-----------------------------------------------------------------------
What are those folders for ?
-----------------------------------------------------------------------
In the beginning you will see these folders:
    ./source        source code, including some internal libraries.

These folders are build tools for linux only. you are not supposed to change it.
    ./site_scons    plugins of scons.
    ./tools         scons builder. some script to help build on linux server. 

After a sucessful build, you will find:
    ./.temp         objects files, qt precompiled files, etc.
    ./build         excutable files.