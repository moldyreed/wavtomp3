# wavtomp3

A tool which encodes 8/16/24/32 bit Wave files into MP3 files using LAME encoder.

##Usage
```
wavtomp3 /path/to/wav/files
```

##Requirements
```
gcc >= 5.3
cmake >= 2.8
git
mingw >= 5.3 \\ windows gcc
```
##Building *nix
```
git clone https://github.com/sickevilfuck/wavtomp3.git --recursive
cd wavtomp3
mkdir build
cd build
cmake ..
make
```

##Building Windows
```
git clone https://github.com/sickevilfuck/wavtomp3.git --recursive
cd wavtomp3
mkdir build
cd build
cmake "-GCodeBlocks - MinGW Makefiles" -DCMAKE_CXX_COMPILER:STRING=/path/to/g++/exe ..
cmake --build .
```
