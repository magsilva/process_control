#include <features.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define HAS_PROPER_STD_LIB defined(__GLIBC__) && __GLIBC__ >= 2 &&  __GLIBC_MINOR__ >= 30
#define SHOULD_USE_STD_LIB 0

/**
 * gettid() returns the caller's thread ID (TID). In a single-threaded
 * process, the thread ID is equal to the process ID (PID, as returned
 * by getpid(2)). In a multithreaded process, all threads have the same
 * PID, but each one has a unique TID.
 *
 * Please note that the thread ID returned by this call is not the same
 * thing as a POSIX thread ID (i.e., the opaque value returned by
 * pthread_self()).
 *
 * @return Thread ID of the calling process.
 */
pid_t system_call_get_thread_id()
{
	pid_t tid;

#if HAS_PROPER_STD_LIB && SHOULD_USE_STD_LIB
	tid = gettid();
#else
	tid = syscall(SYS_gettid);
#endif
	return tid;
}

pid_t system_call_get_process_id()
{
	pid_t pid;

#if HAS_PROPER_STD_LIB && SHOULD_USE_STD_LIB
	pid = getpid();
#else
	pid = syscall(SYS_getpid);
#endif
	return pid;
}

pid_t system_call_get_parent_process_id()
{
	pid_t pid;

#if HAS_PROPER_STD_LIB && SHOULD_USE_STD_LIB
	pid = getppid();
#else
	pid = syscall(SYS_getppid);
#endif
	return pid;
}

int main(int argc, char * argv[])
{
	printf("Using GNU libc major version %d and minor version %d\n", __GLIBC__,  __GLIBC_MINOR__);
	printf("Parent process id: %ld\n", system_call_get_parent_process_id());
	printf("Process id: %ld\n", system_call_get_process_id());
	printf("Thread id: %ld\n", system_call_get_thread_id());
	return EXIT_SUCCESS;
}

