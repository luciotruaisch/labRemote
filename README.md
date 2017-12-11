# Small library to control and read out lab equipment.
Forked from [braramsel](https://github.com/baramsel/labRemote).
Requirements:
  * Qt 5.9.2 [link](https://www.qt.io/download-qt-for-application-development)
  * OpenCV 3.3.1 [Install in Linux](https://docs.opencv.org/3.3.1/d7/d9f/tutorial_linux_install.html)
  * gclib [link](http://galilmc.com/sw/pub/all/doc/gclib/html/osx.html)

To start with:
```shell
git clone --recursive https://github.com/xju2/labRemote
cd src; mkdir build; cd build
cmake ..
make -j4
```
Open **Qt Creator** to import the project: _gui/WaferProberGUI/WaferProbeGUI.pro_
