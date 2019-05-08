# Small library to control and read out lab equipment.

## Commiting code
Create new branch with your initials and short description, for example `jd_mybranch`. When creating merge request make sure to check the box with delete branch after merging.

## Compile code
To start with:
```shell
mkdir build; cd build
cmake ..
make -j4
```

## New projects
You can add labRemote as a submodule and create a new project.
All projects should be [labRemote-apps](https://gitlab.cern.ch/berkeleylab/labremote-apps)
Example project is:
[ExampleLabRemoteProject](https://gitlab.cern.ch/berkeleylab/labremote-apps/examplelabremoteproject)

## Probe Station

Requirements for probe station controller:
 * Qt 5.9.2 [link](https://www.qt.io/download-qt-for-application-development)
 * OpenCV 3.3.1 [Install in Linux](https://docs.opencv.org/3.3.1/d7/d9f/tutorial_linux_install.html)
 * gclib [link](http://galilmc.com/sw/pub/all/doc/gclib/html/osx.html)

Requirements for FTDICom:
 * [libftdi](https://www.intra2net.com/en/developer/libftdi/download.php) - Generic FTDI library
 * [libmpsse](https://github.com/l29ah/libmpsse) - Library that implements common communication protocols (I2C, SPI) using the MPSSE framework on FTDI chips
  
