# Threading Building Blocks (Intel® TBB)

https://www.threadingbuildingblocks.com/  - Comercial
https://www.threadingbuildingblocks.org/  - Apache 2.0 (since 2017)

- Latest [2018_U5 - tbb2018_20180618oss](https://github.com/01org/tbb/archive/2018_U5.tar.gz)

- Old Stable [v4.4 update 6 - tbb44_20160803oss](https://www.threadingbuildingblocks.org/sites/default/files/software_releases/source/tbb44_20160803oss_src.tgz)

## Install with Package

Centos7:

```Bash
yun install tbb-devel
```

OSX (2018_U5):

```Bash
brew install tbb
```

## Build from source

### Build in Linux

```Bash
make -C tbb_$(TBB_VERSION)
```

### Build in Windows

```Bash
build tbb_$TBB_VERSION\build\vs2010\makefile.sln
```

## Reference Articles

1. [Intel TBB vs Boost](https://stackoverflow.com/questions/7130020/intel-tbb-vs-boost)

2. [TBB vs. C++11 concurrency](https://software.intel.com/en-us/forums/intel-threading-building-blocks/topic/278721)

3. [Intel TBB on Raspberry Pi](https://www.theimpossiblecode.com/blog/intel-tbb-on-raspberry-pi/)
