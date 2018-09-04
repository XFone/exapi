# Corvusoft's Restbed 

> Corvusoft's Restbed framework brings asynchronous RESTful functionality to C++11 applications.
> http://www.corvusoft.co.uk
> (dual licensed) AGPL

[source](https://github.com/Corvusoft/restbed)

Latest revision: [v4.6](https://github.com/Corvusoft/restbed/archive/4.6.tar.gz)

## Build from Source

```Bash
wget -c -O restbed-4.6.tar.gz https://github.com/Corvusoft/restbed/archive/4.6.tar.gz
# wget -c https://github.com/Corvusoft/asio-dependency/archive/asio-1-12-0.tar.gz
git clone https://github.com/Corvusoft/asio-dependency
git clone https://github.com/Corvusoft/kashmir-dependency
git clone https://github.com/Corvusoft/catch-dependency
```

patch the source:

```Bash
tar zxf restbed-4.6.tar.gz
cd restbed-4.6
patch -p1 < ../restbed-4.6.patch
```

### Build in MacOS

```Bash
# brew install cmake
cd $RESTBED_SRC
mkdir build
cd build && cmake .. -DBUILD_TESTS=NO
make -j 4
```

### Build in Linux (Ubuntu)

```Bash
cd $RESTBED_SRC
mkdir build
cd build && cmake .. -DBUILD_TESTS=NO
make -j $(nproc)

```

### Build in Windows (VS2015)

```Powershell
```
