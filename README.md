# wavtomp3

A tool that encodes 8/16/24/32 bit Wave files into MP3 with default setting using LAME encoder.

##Usage
```
wavtomp3 /path/to/wav/files
```

##Requrements
```
gcc >= 5.3
cmake >= 2.8
git
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

Before build ensure that you have gcc compiler in PATH environment

```
git clone https://github.com/sickevilfuck/wavtomp3.git --recursive
cd wavtomp3
mkdir build
cd build
cmake ..
make
```
