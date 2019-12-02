# About

This is a mod from MadScientist's masterpiece CCCaster, named "CCCaster Statistics".

In simple words, the CCCaster Statistics mod modifies the CCCaster application to capture every online match result, process and send the data to a online database.
The data sent, are:

- Choosen Characters and Moons
- Match result for each player (won rounds)
- Player's battle keys (for player data binding)

CCCaster Statistics its compatible with the CCCaster v3.0a.020 (unofficial) version. However, the match data will not be recorded.
This project was idealized by the brazillian Melty Blood player Arkhar, and supported by the communities MeltyBrasil and MeltySud. Its an open-souce project.

More info can be found at: [Melty Stats](https://meltystats.com/welcome)

# MadScientist original documentation

Netplay tool for Melty Blood Actress Again Current Code

    Public domain, no license, no warranty. Here be dragons.

    Needs MingW to compile, see Makefile for all build targets.

    scripts/server.py is the UDP tunnelling relay server.
    (The server IPs are currently hardcoded in SmartSocket.cpp)


Install and using:

    Make sure to copy EVERYTHING into the MBAACC folder, including the cccaster folder.

    The GRP folder is the fix for the missing stage select graphics, credits to Erkz.

    Run Add_Handler_Protocol.bat once as admin to launch directly from http://seemeinmelty.meteor.com/

    If you use an analog controller, make sure to adjust the deadzone, else you might get weird inputs.


Hotkeys:

    F4 opens/closes the overlay menu for setting and changing controllers.

    Ctrl + Number changes the delay.

    Alt + Number changes the rollback.

    Spacebar toggles fast-forward when spectating.

    Left/Right + FN2 resets to the respective corners in training mode.

    Holding FN2 after a reset swaps the P1 and P2 positions.

    Down + FN2 resets but swaps P1 and P2 positions.


How to host without port forward:

    Host on any port normally (you can use port 0 to pick any available port).

    Then the client connects to the same provided IP-address:port.

    Relaying is automatically performed, however it is not guaranteed to work for all networks.

    Some routers may be too restrictive, in which case you must open a port to netplay.


Running under Wine:

    First you need to create a 32 bit wine prefix:

        rm -rf ~/.wine

        WINEARCH=win32 WINEPREFIX=~/.wine winecfg

    Also you need install native D3DX9 dlls using winetricks:

        wget http://kegel.com/wine/winetricks

        sh winetricks d3dx9

    Caveats:

        No in-game button config unless someone can figure out how to hook DirectX9 under Wine.

        Can't save replays because MBAA.exe crashes when saving replays under Wine.
