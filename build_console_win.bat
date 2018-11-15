call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars64.bat"

set JOM="C:\Qt\Qt5.10.1\5.10.1\msvc2017_64\bin\jom.exe"
set QMAKE="C:\Qt\Qt5.10.1\5.10.1\msvc2017_64\bin\qmake.exe"

%QMAKE% DIE_source/console/console.pro -spec win32-msvc
%JOM% -j 8 -f Makefile.Release clean
%JOM% -j 8 -f Makefile.Release

del Makefile
del Makefile.Release
del Makefile.Debug
cd debug
del *.obj
del moc_*
cd ..
cd release
del *.obj
del moc_*
cd ..