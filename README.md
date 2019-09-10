# Veracrypt Runner
This Krunner plugin allows you to easily mount/unmount configured Veracrypt volumes.  
Additionally it provides pass integration.

### Required Dependencies  
Debian/Ubuntu:  
`sudo apt install cmake extra-cmake-modules build-essential libkf5runner-dev libkf5textwidgets-dev qtdeclarative5-dev gettext xdotool wmctrl`

openSUSE  
`sudo zypper install cmake extra-cmake-modules libQt5Widgets5 libQt5Core5 libqt5-qtlocation-devel ki18n-devel ktextwidgets-devel kservice-devel krunner-devel gettext-tools xdotool wmctrl`  

Fedora  
`sudo dnf install cmake extra-cmake-modules kf5-ki18n-devel kf5-kservice-devel kf5-krunner-devel kf5-ktextwidgets-devel gettext xdotool wmctrl`  

### Build instructions  

The easiest way to install is:  
`curl https://raw.githubusercontent.com/alex1701c/VeracryptRunner/master/install.sh | bash`

Or you can do it manually:  
```
git clone https://github.com/alex1701c/VeracryptRunner
cd VeracryptRunner
mkdir build
cd build
cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` ..
make
sudo make install
kquitapp5 krunner 2> /dev/null; kstart5 --windowclass krunner krunner > /dev/null 2>&1 &
# Launch system settings module to configure Volumes
kcmshell5 kcm_plasmasearch &
```

### Screenshots  

#### Show options  
![Show options](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VeracryptRunner/run_options.png)

#### Lauch Veracrypt and fill in password using pass integration  
![Launch Veracrypt with pass integration](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VeracryptRunner/mount_with_pass_integration.gif)

#### Unmount option
![Unmount option](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VeracryptRunner/unmount_option.png)

#### Config dialog  
![Config dialog](https://raw.githubusercontent.com/alex1701c/Screenshots/master/VeracryptRunner/config_overview.png)
