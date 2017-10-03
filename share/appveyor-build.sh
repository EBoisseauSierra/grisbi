#!/bin/sh

source /appveyor.environment
export MSYSTEM

cd /c/projects/grisbi-src
./autogen.sh
./configure --prefix /c/projects/grisbi-inst/ --enable-static --disable-shared --disable-installed --disable-modules --disable-delegate-build --enable-zero-configuration

v=$(grep PACKAGE_VERSION config.h | cut -f2 -d '"')
powershell.exe -command "Update-AppveyorBuild -Version \"$v\""
# -B%APPVEYOR_BUILD_NUMBER%\""

make -j 2

make install

ldd /c/projects/grisbi-inst/bin/grisbi.exe

cd /nsis-3.02.1
./makensis.exe /c/projects/grisbi-src/share/grisbi.nsi
