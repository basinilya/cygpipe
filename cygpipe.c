#include <w32api/windows.h>
#include <sys/cygwin.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

#define BUFSZ 100

int main(int argc, char *argv[]) {
  HANDLE hPipe;
  int fds[2];
  char procfs_path[BUFSZ], pipename[BUFSZ] = "";

  pipe(fds);

  sprintf(procfs_path, "/proc/self/fd/%d", fds[0]);
  readlink(procfs_path, pipename, BUFSZ);
  close(fds[0]);
  close(fds[1]);
  
  //fprintf(stderr, "pipename: %s\n", pipename);

  fprintf(stderr, "named pipe: %s\n", argv[1]);

  hPipe = CreateNamedPipeA(argv[1], PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_WAIT, 1, 2048, 2048, 0, NULL);
  if (hPipe == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "CreateNamedPipe() failed: %d\n", (int)GetLastError());
    exit(1);
  }
  fprintf(stderr, "CreateNamedPipe() succeeded\n");

  if (!ConnectNamedPipe(hPipe, NULL) && GetLastError() != ERROR_PIPE_CONNECTED) {
    fprintf(stderr, "ConnectNamedPipe() failed: %d", (int)GetLastError());
    exit(1);
  }
  fprintf(stderr, "ConnectNamedPipe() succeeded\n");

  if (3 != cygwin_attach_handle_to_fd(pipename, 3, hPipe, 1, GENERIC_WRITE | GENERIC_READ)) {
    fprintf(stderr, "cygwin_attach_handle_to_fd() failed");
    exit(1);
  }
  fprintf(stderr, "cygwin_attach_handle_to_fd() succeeded\n");

  Sleep(10000);

#if 0
#endif
  return 0;
}

