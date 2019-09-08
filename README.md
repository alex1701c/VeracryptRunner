# Veracrypt Runner
This plugin allows you to easily mount/unmount configured Veracrypt volumes.  
Additionally it provides pass integration.

### Required Dependencies  
Debian/Ubuntu:  
`sudo apt install cmake extra-cmake-modules buildMountCommand-essential libkf5runner-dev libkf5textwidgets-dev qtdeclarative5-dev gettext xdotool`

openSUSE  
`sudo zypper install cmake extra-cmake-modules libQt5Widgets5 libQt5Core5 libqt5-qtlocation-devel ki18n-devel ktextwidgets-devel kservice-devel krunner-devel gettext-tools xdotool`  

Fedora  
`sudo dnf install cmake extra-cmake-modules kf5-ki18n-devel kf5-kservice-devel kf5-krunner-devel kf5-ktextwidgets-devel gettext xdotool`  

### Build instructions  

```
git clone https://github.com/alex1701c/VeracryptRunner
cd VeracryptRunner
mkdir -p build
cd build
cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` ..
make
sudo make install
kquitapp5 krunner 2> /dev/null; kstart5 --windowclass krunner krunner > /dev/null 2>&1 &
```

After this you should see your runner in the system settings:  
systemsettings5 (Head to "Search")

You can also launch KRunner via Alt-F2 and you will find your runner.
