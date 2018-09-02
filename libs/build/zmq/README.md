# ZeroMQ

[Home - zeromq.org](http://zeromq.org/)

Current revision: [4.2.3](https://github.com/zeromq/libzmq/releases/download/v4.2.3/zeromq-4.2.3.tar.gz)

Legacy version: [4.1.6](https://github.com/zeromq/zeromq4-1/archive/v4.1.6.tar.gz)

Old version: [3.2.5](http://download.zeromq.org/zeromq-3.2.5.tar.gz)

## Build from Source

### Build in Linux or MacOS

```Bash
cd zeromq-$VER
./autogen.sh
./configure --without-libsodium
make
```

### Build in Windows

```powershell
build .\zeromq-$VER\builds\msvc\msvc10.sln
```
