# Zex2 - 3D Space Trading Game with in-cockpit view

Summary: OpenGL, traditional first person view, quaternion based engine - RNG procedural generation, trading, stack based AI management, motherships, ship upgrades, wingmen etc.

**NOTE: PORTING IS IN PROGRESS - THIS GAME DOES NOT ON MODERN PLATFORMS RUN YET!!!**

## BACKGROUND

Zex 2 is space-trading game with 3D graphics rendered with OpenGL, viewed from the cockpit.

It was written by Stuart Ball and Rob Probin, starting in 2000 and was based on the Zex source code initially, but generally is quite different in most places.

In 2016 we considered making the source code public, but other projects got in the way. At the start of 2018 we started again.
 
The game and manual probably refer to lightsoft.co.uk - but that web site doesn’t really exist except as a placeholder currently.

This repository history represents "ZEX2.3", the third 'clean-up', which goes somewhat to to explaining why the history stops at 2003. As you might be able to tell this project went from cvs to svn to git. We do have access to the old repos - but no one is missing anything :-)

## REPOSITORY OVERVIEW

All source code is available and released as open source.

Project files are provided for Mac OS, with Windows and Linux to follow. Porting to SDL2 is in progress. After that you might need to squish some bugs…

.ZLV files are level files and are text files.

.z files are 3D model “mesh” files (exported with a program called ‘Meshworks’).

RAW are raw texture files - and are binary.

.rbs are various files that were copied from the Mac resources. These can be considered binary files, but in reality things are not that simple :-) These contain text, graphics and other data.

Sadly, no original graphics and model source files are present in the repository currently, but reverse engineering the rbs files is probably possible. We probably have the originals somewhere on a CD…

## LICENSES

Code GPLv3 (Share-a-like, patent grant, etc.) 

Graphics - source not supplied, but:
    Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
(Contact us if you need other terms).

Please only use the ‘Zex’ and 'Zex 2' names - to represent the original game and ‘Lightsoft’ logos (including fonts, rainbows, and Anvil) to represent only the trading name of original authors (Stuart Ball and Robert Probin). We consider these trademarks - although distribution with game is permitted as long as you don’t try to trade with these logos or otherwise represent them as your own.


Documentation:
  Attribution-NonCommercial-ShareAlike 4.0 International
(Contact us if you need other terms).

There are various SDL libraries in the repository  - these have their own license. See the files or http://libsdl.org/


## OUTSTANDING CHANGES REQUIRED

There are quite a few things that need doing. We will document these properly when the code is in a runnable state. At the moment the list is summarised as:

 * Get the code compiling and linking.
 * Get the project running to allow incremental porting of system call features.
 * Finish the Mac system call emulation layer, including SDL2 porting.
 * Ensure OpenGL renders to screen.
 * Replace/Port Coffee (PowerPC subroutine threaded Forth used for game scripting).
 * Windows/Linux projects
 * Increment testing and fixing

## BINARIES

When we have something that runs, we plan to make a prebuilt binary for one or more platforms.


