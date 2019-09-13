#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include  "procdef.h"
#include  "glovaldef.h"
#include  "funcdef.h"

void
cre_sock(void)
{
  int rtn;
  int udpbfz = UDPRCVBUFSZ;

  rcvinfo.sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (rcvinfo.sock < 0)
  {
    fprintf(stderr, "r socket : %s\n", strerror(errno));
    exit(1);
  }

  rtn = setsockopt(rcvinfo.sock, SOL_SOCKET, SO_RCVBUF, &udpbfz, sizeof(udpbfz));
  if (rtn < 0)
  {
    fprintf(stderr, "setsockopt SO_RCVBUF %d : %s\n", udpbfz, strerror(errno));
    exit(1);
  }
  rtn = bind(rcvinfo.sock, (struct sockaddr *)&rcvinfo.src_addr, sizeof(struct sockaddr_in));
  if (rtn < 0)
  {
    fprintf(stderr, "bind : %s\n", strerror(errno));
    exit(1);
  }
}
void
init_proc(void)
{
  cre_sock();
}
