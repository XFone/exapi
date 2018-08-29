#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

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
  fprintf(stderr, "Thread [0x%x] got signal %d:\n", (unsigned)tid, sig); 
  backtrace_symbols_fd(array, size, STDERR_FILENO);

  if (_thread_local_sigsegv_handler) _thread_local_sigsegv_handler(sig, si, ctx);

  // xiaofeng 2015-03-20: only exit the current thread
  //exit(status);
  pthread_exit(&status);
}

void catch_segfault(void (*_handler)(int, siginfo_t *, void *)) 
{
    _thread_local_sigsegv_handler = _handler;
    signal(SIGSEGV, (void (*)(int))handler); 
}
