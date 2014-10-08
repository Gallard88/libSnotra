#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>

#include "Snotra.h"

#define SOCK_PATH "/tmp/.Snotra.Socket"

// ==============================================
// ==============================================

static int Snotra_Fd;

int Snotra_Connect(void)
{
  int len;
  struct sockaddr_un remote;

  if ((Snotra_Fd = socket(AF_UNIX, SOCK_SEQPACKET, 0)) == -1) {
    perror("socket");
    return -1;
  }

  remote.sun_family = AF_UNIX;
  strcpy(remote.sun_path, SOCK_PATH);
  len = strlen(remote.sun_path) + sizeof(remote.sun_family);
  if (connect(Snotra_Fd, (struct sockaddr *)&remote, len) == -1) {
    perror("connect");
    return -1;
  }
  return Snotra_Fd;
}

// ==============================================
int Snotra_Send(const char *module, const char *parameter, float value)
{
  char  *buf;
  int bytes;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  if ( Snotra_Fd < 0 ) {
    return -1;
  }

  bytes = asprintf(&buf, "{\"Module\":\"%s\", \"Date\":\"%04d-%02d-%02d, %02d:%02d:%02d\",\"Parameter\":\"%s\", \"Value\": %lf }",
        module,
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
        parameter,
        value);

  if (( buf != NULL ) && ( bytes > 0 )) {
    if (send(Snotra_Fd, buf, strlen(buf), 0) == -1) {
      perror("send");
      return -1;
    }
    free(buf);
    return 0;
  } else {
    return -1;
  }
}

// ==============================================
void Snotra_Close(void)
{
  close(Snotra_Fd);
  Snotra_Fd = -1;
}

// ==============================================
// ==============================================

