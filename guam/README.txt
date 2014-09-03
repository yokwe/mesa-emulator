Summary
-------
This is XEROX MESA processor emulator based on APilot/15.3 Guam.
The emulator can run on Linux (i686, arm = raspberry pi) and Windows (win32).
The emulator can use disk image of GlobalView for Windows.


Emulator
--------
You need following files to run emulator.
  Disk Image
  Germ File
You can use Disk Image and Germ File used of Dawn
  Dawn.dsk and Dawn.germ
Or you can use Disk Image and Germ File of GlobalView for Windows.
  GVWin001.dsk and GVWIN.GRM

Linux binary has network support. So you can echo test or remote debug with other machine.
Windows binary has no network support.

CoProcessor stream is not implemented because of no information.
So you cannot use feature (PC File Access) that use CoProcessor Stream in GVWin.

TCP is not worked as expected even using dedicated network interface.


Verified build environment
--------------------------
Ubuntu 14.04.1 32 bit
  native build with qt4
  native build with qt5
  raspberry pi cross build with qt4
  win32 cross build with qt4
  
Raspberry Pi cross build needs cross compile tool at /opt/raspi/tools.
  git://github.com/raspberrypi/tools.git

win32 cross build needs MXE at /opt/mxe.
  git://github.com/mxe/mxe.git


Library Dependency
------------------
qt4     - 4.8.6
qt5     - 5.2.1
log4cpp - 0.3.5
cppunit - 1.13.1

You need qt4 or qt5(preferred).
You need compile log4cpp and install it in MXE


Acknowledgement
---------------
Without information from Don Woodward, I cannot finish this program.
Especially correction of PrincOps and source code of agent.


License
-------
This software is released under the BSD License, see LICENSE.txt.


Disclaimer
----------
All trademarks and registered trademarks mentioned herein are
the property of their respective owners.


// Yasuhiro Hasegawa AKA Hasegawa:FXIS:Fuji Xerox