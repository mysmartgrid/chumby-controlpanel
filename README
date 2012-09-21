An alternative graphical interface for the chumby.

Build
=====

    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../modules/Toolchain-arm.cmake ../
    make
    make package

Install
=======

Unpack the archive in build on an USB-Stick (the debugchumby file as to be in the root directory of the stick).
Plug the stick into your chumby.
Reboot your chumby.

Debug and Development
=====================

The code also runs on intel based machines with an X11-Server.

Build
-----

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ../
    make

Run
---

    ./controlpanel/chumby-controlpanel

You can set the following environment variables:
* CHUMBY_PLUGIN_DIR to set the directory containing the controlpanel plugins
* CHUMBY_SOUND_CARD to set the alsa audio device used by the controlpanel

For a better experience use Xephyr:
    Xephyr -screen 320x240 :1 &
    DISPLAY :1 ./controlpanel/chumby-controlpanel
