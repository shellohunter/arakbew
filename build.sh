
export Custom_QT4DIR="/proj/mtk70628/WS_MTK70628_TEMP/BDP/Customer_Philips/BDP_V301_7118_8580_Golden_7675/BDP_Linux/oss/library/qt"
export PKG_CONFIG_PATH=$Custom_QT4DIR"lib/pkgconfig"
export LD_LIBRARY_PATH="/usr/local/python2.7/lib/"

#echo $Custom_QT4DIR
#echo $PKG_CONFIG_PATH
#/usr/local/python2.7/bin/python -V

/usr/local/python2.7/bin/python tools/scons-2.1.0/bin/scons -Q

