#!/bin/bash

if [[ $(basename "$PWD") != "VeracryptRunner"* ]];then
    git clone https://github.com/alex1701c/VeracryptRunner
    cd VeracryptRunner/
fi

mkdir -p build
cd build
cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` ..
make -j2
sudo make install

kquitapp5 krunner 2> /dev/null
kstart5 --windowclass krunner krunner > /dev/null 2>&1 &

# Launch plasma plugin config dialog to configure volumes
kcmshell5 kcm_plasmasearch &

echo "Installation finished !";
