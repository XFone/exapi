# This library provides an iconv() implementation.

Homepage: [gnu.org - libiconv](http://www.gnu.org/software/libiconv)

Download: [v1.14](http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.14.tar.gz)

## Build from Source

### Build in Windows

[HowToBuildLibiconv](http://www.codeproject.com/Articles/302012/How-to-Build-libiconv-with-Microsoft-Visual-Studio)

### Build in CentOS 7

Compilation error in gcc:
> ./stdio.h:1010:1: error: 'gets' undeclared here (not in a function)

```Bash
wget -c http://www.itkb.ro/userfiles/file/libiconv-glibc-2.16.patch.gz
cd libiconv-1.14/srclib
patch -p1 < libiconv-glibc-2.16.patch
```
