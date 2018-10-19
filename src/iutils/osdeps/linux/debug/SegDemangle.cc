#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#ifdef __GNUG__
# include <cxxabi.h>
#endif

#include <string>
#include <iostream>

using namespace std;

string demangle(const char *symbol) {
  #ifdef __GNUG__
    // try to demangle a c++ name
    //
    // 0   test-binance-spi                    0x000000010b0cc73d _Z7handleriP9__siginfoPv + 61
    // 1   libsystem_platform.dylib            0x00007fff777bfb3d _sigtramp + 29
    // <id> <prog>                             <addr>             <name>    <offset>
    //
    #define NAME_POS    60

    if (strlen(symbol) > NAME_POS) {
        size_t size;
        int   status;
        char* demangled;
        string temp(symbol + NAME_POS - 1);
        size_t pos = temp.find(' ');
        string name = temp.substr(0, pos);
        string offset = temp.substr(pos);
        if (NULL != (demangled = abi::__cxa_demangle(name.c_str(), NULL, &size, &status))) {
            string result(symbol, NAME_POS);
            result += demangled;  free(demangled);
            result += offset;
            return result;
        }
    }
  #endif

   // try to get a regular c symbol
   return symbol;
}

// see http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
//
// MUST compile wtih $ gcc -g -rdynamic
/* __thread */ void (*_thread_local_sigsegv_handler)(int, siginfo_t *, void *) = 0;

void handler(int sig, siginfo_t *si, void *ctx) {
    void *array[10];
    size_t size;
    int status = EXIT_FAILURE;
    pthread_t tid = pthread_self();

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    // (BUG here) SHOULD use write()
    fprintf(stderr, "Thread [%p] got signal %d:\n", tid, sig); 
    char **strings = backtrace_symbols(array, size);

    if (strings != nullptr) {
        for (int i = 0; i < size; ++i) {
            cerr << demangle(strings[i]) << endl;
        }
    }
    free(strings);

    if (_thread_local_sigsegv_handler) _thread_local_sigsegv_handler(sig, si, ctx);

    // xiaofeng 2015-03-20: only exit the current thread
    pthread_exit(&status);
}

void catch_segfault_cpp(void (*_handler)(int, siginfo_t *, void *)) 
{
    _thread_local_sigsegv_handler = _handler;
    signal(SIGSEGV, (void (*)(int))handler); 
}
