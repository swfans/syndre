# syndre

**BullfrogGame Port**, alternative binary for the classic Bullfrog game.

This is a generic framework which can be used as base when preparing ASM port
of any Bullfrog game.

## About

**BullfrogGame Port** is a port of the 1990s DOS game BullfrogGame to modern
operating systems. In particular, it runs on GNU, Mac OS and Windows, but it
should also work on any system supported by the SDL library on the i386
architecture.

The port was created by disassembling the original DOS executable and replacing
the DOS-specific parts with platform-independent code, using [SDL](https://www.libsdl.org/)
for input and video display, [Vorbis](https://xiph.org/vorbis/) for music and
[OpenAL](https://www.openal.org/) for sound.

Only the modernised executable is provided with this port. This means that game
data are not included. To install the **BullfrogGame Port**, you will need to
have the original data from either *BullfrogGame CD*, or digital distribution.

## Installation

To install **BullfrogGame Port**, you will need either the original *BullfrogGame
PC CD*, or digital distribution of the game. There were several physical
versions released. It is uncertain whether this port will work with all
releases, so choose the most popular, english version.

The CD version which you use narrows the range of available languages.

* Multi-lingual CD contains `eng` `fre` `ita` `spa` `swe`,
* German release is required to use `ger`,

The steps you need to take vary depending on your operating system.

### Installing on GNU or UNIX

If you're installing on GNU or UNIX, you will have to compile from source.
The steps are:

1. download and unpack the source tarball or clone git repo
2. follow [building instructions](#general-building-instructions) below to get
   a compiled executable
3. after the build succeeded, do `make install` in the directory where build
   commands were executed, to copy built files into an installation folder
   (`/usr/local/share/syndre` by default)
4. insert the *BullfrogGame CD* and make sure it is mounted (e.g. in `/media/cdrom`)
5. do `util/install -f SOURCE -t TARGET -l LANG`, where
   * *SOURCE* is the game CD path, like `/media/cdrom`,
   * *TARGET* is the destination path, in this case `/usr/local/share/syndre`,
   * *LANG* is the installation language, and can be one of: `eng` `fre` `ger` `ita` `spa` `swe`.

If all went well, you can now play the game.

### Installing on Mac OS X

There is support for creating Mac OS X bundles. While you can just install
the game into the UNIX directory structure, using a bundle is a cleaner solution.

#### Making a Mac OS app bundle

If you build an application bundle, you can install the game by just drag-and-drop
into your *Applications* directory.

You can create the bundle by following these instructions:

1. download and unpack the source tarball or clone git repo
2. follow [building instructions](#building-on-mac-os-x) below to get
   a compiled executable
3. after the code is compiled, install the data, as explained in the
   [GNU or UNIX](#installing-on-gnu-or-unix) section, to any directory,
   say `./data` (in case you wonder, the CD drive should be mounted under
   `/Volumes/SYN WARS QA 9.0`)
4. finally, run `util/mkbundle ./data` (replacing `./data` with the directory
   you installed the data to); this will produce an application bundle named
   `BullfrogGame.app` in the current directory
5. if you want there to be a pretty icon for the bundle, copy `res/syndre.icns`
   to `BullfrogGame.app/Contents/Resources`

### Installing on Windows

There are two ways to install **BullfrogGame Port** on Windows - either download
the installer, or download the source code and compile it by hand.
It is recommended to use the installer!

The installer requires the original *BullfrogGame CD* to be present in the
CD-ROM drive. It will copy the files from the original *BullfrogGame CD* and
optionally encode the game music from the CD to ogg files used by this port.

The *BullfrogGame CD* is not required to play the game, it is used only during
the installation process.

If you've decided on the hand-compilation option, proceed with the following steps:

1. download and unpack the source tarball or clone git repo
2. follow [building instructions](#building-on-windows) below to get
   a compiled executable and configuration files
3. copy all the files and directories, except for the Language directory, from
   the `Game` directory within the *BullfrogGame CD* to the location of your
   desire, say `SyndRELocation`.
4. copy all the files, except `sound.dat`, from the `game\language\LANGUAGE\` from
   the *BullfrogGame CD* to the `SyndRELocation\Data` directory, where *LANGUAGE*
   stands for the appropriate subdirectory fo the language you want
5. copy the `sound.dat` file (from the directory stated above) to the
   `SyndRELocation\sound` directory
6. *[optional]* rip the game CD Audio tracks to `track_{1,2,3}.ogg` (vorbis)
7. *[optional]* create a `SyndRELocation\music` directory and copy the
   previously encoded ogg files there
8. from the compilation folder, copy `syndre.exe` and `conf` folder to your
   `SyndRELocation` directory

If instead of the above points you prefer more technical description on what to
copy and where, read the script `util/install` which is used to perform
installation on GNU or UNIX systems. If your Windows has a Linux subsystem
installed, you may even use it to perform the installation automatically - see
[GNU or UNIX chapter](#installing-on-gnu-or-unix) for details.

You can now launch `syndre.exe` and have fun!

## Building

Note that you can oly build the port for x86 architecture, and you either need
32-bit OS, or 64-bit OS with 32-bit user space libraries available.

### General building instructions

To build **BullfrogGame Port**, you will need the following:

* GNU Autotools
* GNU C compiler
* Python 3 (with module: polib)
* vorbis-tools (oggenc in particular)
* cdparanoia
* development versions of the following libraries:
  * SDL2 or SDL1.2
  * OpenAL (we recommend OpenAL Soft)
  * libvorbis
  * libogg
  * libpng
  * zlib
  * WildMIDI

Once you've made sure you have the above, proceed with the following steps:

1. go into the directory with `syndre` source release (containing `conf`, `doc`, `src` etc.)
2. do `autoreconf -if` to create build scripts from templates
3. do `./configure` to make the build scripts find required toolchain and libraries
4. do `make` to compile the executable file

You should now have a working `src/syndre` executable file.

#### Build example - Ubuntu 20.04 64-bit

Here are specific commands required to compile the executable on Ubuntu linux.

Install the dependencies - remember that some must be 32-bit (i386):

```
sudo apt install gcc-multilib g++-multilib lib32z1
sudo apt install python3 python3-polib
sudo apt install vorbis-tools
sudo apt install cdparanoia
sudo apt install libsdl2-dev:i386
sudo apt install libopenal-dev:i386
sudo apt install libvorbis-dev:i386 libvorbisfile3:i386
sudo apt install libogg-dev:i386
sudo apt install libwildmidi-dev:i386
```

Now as our host is ready, we can start working on the actual `syndre` sources.
Go to that folder, and generate build scripts from templates using autotools:

```
autoreconf -ivf
```

Next, proceed with the build steps; we will do that in a separate folder.

```
mkdir -p release; cd release
PKG_CONFIG_PATH="/usr/lib/i386-linux-gnu/pkgconfig" CFLAGS="-m32" CXXFLAGS="-m32" LDFLAGS="-m32" ../configure
make V=1
```

Providing `PKG_CONFIG_PATH` allows us to tell `pkg-config` command where to
search for 32-bit packages (it would use a path to 64-bit ones by default).
The `V=1` variable makes `make` print each command it executes, which makes
diagnosing issues easier.

In case you also want a debug build:

```
mkdir -p debug; cd debug
PKG_CONFIG_PATH="/usr/lib/i386-linux-gnu/pkgconfig" CPPFLAGS="-DDEBUG -D__DEBUG" CFLAGS="-m32 -g -O0 -Wall" CXXFLAGS="-m32 -g -O0 -Wall" LDFLAGS="-m32 -g -O0 -Wall" ../configure
make V=1
```

Explanation of the parameters:

* The `-g -O0` flags make it easier to use a debugger like _GDB_ with the
  binary, by storing symbols and disabling code optimizations.
* The `-Wall` flags enable displaying more warnings during compilation.
* The `-DDEBUG -D__DEBUG` defines make the binary store more information in
  `error.log` file. First one enables verbose logging from application code,
  second does the same for libraries used.
* The flags are set separately for C preprocessor (`CPP`), compilers (`C`, `CXX`)
  and linker (`LD`). See [GNU Automake documentation](https://www.gnu.org/software/automake/manual/html_node/Programs.html)
  for details on that.

Some tests to verify the build are included in the project. To execute them:

```
make V=1 check
```

#### Build example - MSYS2 updated 2023-01 on Windows

Using Minimal System and the MinGW toolchain available within, it is possible
to build the executable using the same way as for UNIX systems, with bash and autotools.

First install the dependencies - mingw32, since we need 32-bit toolchain:

```
pacman -S mingw-w64-i686-binutils mingw-w64-i686-autotools mingw-w64-i686-pkgconf mingw-w64-i686-make mingw-w64-i686-gcc
pacman -S mingw-w64-i686-libpng
pacman -S mingw-w64-i686-SDL2
pacman -S mingw-w64-i686-openal
pacman -S mingw-w64-i686-libvorbis
pacman -S mingw-w64-i686-libogg
```

The WildMIDI library is not available as MSYS2 pacman package, install manually:

```
pacman -S unzip
wget https://github.com/Mindwerks/wildmidi/releases/download/wildmidi-0.4.5/wildmidi-0.4.5-win32.zip
unzip wildmidi-0.4.5-win32.zip
cp wildmidi-0.4.5-win32/*.h /mingw32/include/
cp wildmidi-0.4.5-win32/*.a /mingw32/lib/
cp wildmidi-0.4.5-win32/*.dll /mingw32/bin/
```

The Python interpeter needs to have an additional module installed:

```
pip install polib
```

Make sure the modules are installed on the Python version used during build - with MSYS2,
you often get separate `i686` and `x86_64` versions of Python. For both of them, you
can separately install `pip` package, and then add any missing modules through it.
To select specific `python3` or `pip` at command line, alter the `PATH` variable,
like in some commands in later part of the instructions.

If you do not have any Python, then install the one which fits other `i686` tools:

```
pacman -S mingw-w64-i686-python3 mingw-w64-i686-python-pip
```

Now as our host is ready, we can start working on the actual `syndre` sources.
We will still have to provide paths to 32-bit configuration - MSYS will prefer
folders with data for 64-bit building.
Go to the `syndre` folder, and generate build scripts from templates using autotools:

```
autoreconf -ivf --include=/mingw32/share/aclocal/
```

Next, proceed with the build steps; we will do that in a separate folder.
Note how we are modifying PATH and PKG_CONFIG_PATH environment variables to make shell
search for mingw32 binaries and pkg-config search for mingw32 packages before
the default mingw64 ones:

```
mkdir -p release; cd release
PATH="/mingw32/bin:$PATH" PKG_CONFIG_PATH="/mingw32/lib/pkgconfig" CFLAGS="-m32" CXXFLAGS="-m32" LDFLAGS="-m32" ../configure --prefix=/mingw32
PATH="/mingw32/bin:$PATH" make V=1
```

Depending on versions of packages, your `make` executable may be also called `mingw32-make`.
Also, the build system may expect to find `python` binary within `mingw32` folder,
failing to find it if you have it somewhere else. In both cases, just creating a symbolic
link with name expected by build system will fix the issue.

### Building on Mac OS X

Mac OS X is at its core a UNIX system. To build the **BullfrogGame Port** it is
enough to follow the [general building instructions](#general-building-instructions).

The GCC compiler for Mac OS X comes as part of XCode, which you can get from
Apple's web site. You can also make your life a lot easier by using
[Fink](http://www.finkproject.org/) or [MacPorts](http://www.macports.org/)
to install the required packages for you, instead of having to download and
compile them manually.

If you have Mac OS 10.6 ("Snow Leopard") or newer, you will need to make sure
that your libraries contain code for the i386 architecture. In MacPorts this
is achieved by using the `+universal` variant (e.g. `port -vu install libsdl2 +universal`).

When you configure your compilation toolchain and location of libraries,
you will need to pass `CFLAGS="-arch i386"`, like so:

```
./configure CFLAGS="-arch i386"
```

If you are planning to create [Mac OS app bundle](#making-a-mac-os-app-bundle),
you will also need to pass `data-path`, so youf final command will be:


```
./configure --with-data-path="BullfrogGame.app/Contents/Resources" CFLAGS="-arch i386"
```

Then, do `make` as the [general building instructions](#general-building-instructions) tell.

## Done

That's all. Have a nice play!
