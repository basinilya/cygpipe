#include <w32api/windows.h>
#include <sys/cygwin.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  HANDLE hPipe, hVictim;
  hPipe = CreateNamedPipeA(argv[1], PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_WAIT, 1, 2048, 2048, 0, NULL);
  if (hPipe == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "CreateNamedPipe() failed\n");
    exit(1);
  }
  fprintf(stderr, "CreateNamedPipe() succeeded\n");
  if (!DuplicateHandle(GetCurrentProcess(), hPipe, GetCurrentProcess(), &hVictim, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
    fprintf(stderr, "DuplicateHandle() failed");
    exit(1);
  }
  fprintf(stderr, "DuplicateHandle() succeeded\n");
  //_IFIFO+
  if (!ConnectNamedPipe(hPipe, NULL) && GetLastError() != ERROR_PIPE_CONNECTED) {
    fprintf(stderr, "ConnectNamedPipe() failed: %d", (int)GetLastError());
    exit(1);
  }
  fprintf(stderr, "ConnectNamedPipe() succeeded\n");
  if (3 != cygwin_attach_handle_to_fd(argv[1], 3, hVictim, 1, GENERIC_WRITE | GENERIC_READ)) {
    fprintf(stderr, "cygwin_attach_handle_to_fd() failed");
    exit(1);
  }
  fprintf(stderr, "cygwin_attach_handle_to_fd() succeeded\n");
  Sleep(100000);
  return 0;
#if 0
#endif
}

