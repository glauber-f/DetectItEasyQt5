DIE-engine
==========

DIE engine modified by zengge especially for VS2017 64Bit and QT 5.X on Windows 10 64 Bit

How to build:
==========
## Windows:

* Install VS2017: https://visualstudio.microsoft.com/zh-hans/vs/
* Install QT 5.10 http://download.qt.io/official_releases/qt/5.10/5.10.1/qt-opensource-windows-x86-5.10.1.exe
* Download Jom: http://download.qt-project.org/official_releases/jom/jom.zip and unpack to c:\QT\jom

* run build_console_win.bat && build_diedll_win.bat && build_dielib_win.bat

## Linux:

* Install QT 5.10: http://download.qt.io/official_releases/qt/5.10/5.10.1/single/qt-everywhere-src-5.10.1.tar.xz
* Edit build_console_lin.sh: Set PATH for QMAKE

* run build_console_lin.sh ( bash -x build_console_lin.sh )

## Mac:

* install XCODE: https://developer.apple.com/xcode/downloads/
* Install QT 5.10: http://download.qt.io/official_releases/qt/5.10/5.10.1/qt-opensource-mac-x64-5.10.1.dmg

* run build_console_mac.sh ( bash -x build_console_mac.sh )


Don't forget after compilation to get signatures from https://github.com/horsicq/Detect-It-Easy (folder "db").
Just place the signatures to project's folder.
