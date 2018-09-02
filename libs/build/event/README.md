# libevent

Latest stable release: [2.1.8](https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz)

## Build in Linux or Darwin

build with GNU Makefile

```Bash
make
```

## Build in Windows

1. start vs2012 x86 "Native Tools Command Prompt"
    ```shell
    cd libevent-$VER
    nmake /f Makefile.nmake
    ```

2. output
    - libevent_core.lib：
        All core event and buffer functionality. This library contains all the event_base, evbuffer, bufferevent, and utility functions.
    - libevent_extras.lib：
        This library defines protocol-specific functionality that you may or may not want for your application, including HTTP, DNS, and RPC.
    - libevent.lib：
        This library exists for historical reasons; it contains the contents of both libevent_core and libevent_extra. You shouldn’t use it; it may go away in a future version of Libevent.

3. include folder
  libevent-$VER\WIN32-Code
  libevent-$VER\include       ===>   .\include

## TODO

Check [libuv](http://libuv.org/) to replace libevent

libuv, libev and libevent
    Redis作者拒绝微软的Windows补丁
    http://www.dedecms.com/knowledge/data-base/nosql/2012/0820/8604.html

libevent - http://libevent.org/
    win_IOCP and unix_epoll

Libev - http://software.schmorp.de/pkg/libev.html
    unix_epoll only

libuv 与 libev 的对比:
    http://blog.csdn.net/nanjunxiao/article/details/9066077
