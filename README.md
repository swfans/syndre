# bfgame

BullfrogGame port, alternative binary for the classic Bullfrog game

This is a generic framework which can be used as base when preparing ASM port
of any Bullfrog game.

## About

BullfrogGame Port is a port of the 1990s DOS game BullfrogGame to modern
operating systems. In particular, it runs on GNU, Mac OS and Windows, but it
should also work on any system supported by the SDL library on the i386
architecture.

The port was created by disassembling the original DOS executable and replacing
the DOS-specific parts with platform-independent code, using SDL for input
and video display, Vorbis for music and OpenAL for sound.

Only the modernised executable is provided with this port. This means that game
data are not included. To install the BullfrogGame Port, you will need to
have the original data from either BullfrogGame CD, or digital distribution.

## Installation

To install BullfrogGame Port, you will need either the original BullfrogGame
PC CD, or digital distribution of the game. There were several physical
versions released. It is uncertain whether this port will work with all
releases, so choose the most popular, english version.

The steps you need to take vary depending on your operating system. If you're
installing on GNU or UNIX, you will have to compile from source. There is
support for creating Mac OS X bundles, and there is an installer for Windows. 

## Building

Note that you can oly build the port for x86 architecture, and you either need
32-bit OS, or 64-bit OS with 32-bit user space libraries available.

### GNU or UNIX

To build BullfrogGame Port on GNU or UNIX, you will need the following:

* GNU C compiler
* Python 3
* vorbis-tools (oggenc in particular)
* cdparanoia
* development versions of the following libraries:
  * SDL
  * OpenAL (we recommend OpenAL Soft)
  * libvorbis
  * libogg
  * libpng
  * zlib

Once you've made sure you have the above, proceed with the following steps:

1. download and unpack the source tarball; go into the newly-created directory
2. do `./configure && make && make install`
3. insert the BullfrogGame CD and make sure it is mounted (e.g. in `/media/cdrom`)
4. do `util/install -f SOURCE -t TARGET -l LANG`, where
   * *SOURCE* is the game CD path, like `/media/cdrom`,
   * *TARGET* is the destination path, in this case `/usr/local/share/bfgame`,
   * *LANG* is the installation language, and can be one of: `eng` `fre` `ita` `spa` `swe`.

You should now have a working executable file.

#### Specific example - Ubuntu 20.04 64-bit

Install the dependencies - remember that some must be 32-bit (i386):

```
sudo apt install gcc-multilib lib32z1 python3
sudo apt install vorbis-tools
sudo apt install cdparanoia
sudo apt install libsdl1.2-dev:i386
sudo apt install libopenal-dev:i386
sudo apt install libvorbis-dev:i386 libvorbisfile3:i386
sudo apt install libogg-dev:i386
```

Now proceed with the build steps:

```
autoreconf
automake --add-missing
CFLAGS=-m32 LDFLAGS=-m32 PKG_CONFIG_PATH=/usr/lib/i386-linux-gnu/pkgconfig ./configure
make
make install
```

