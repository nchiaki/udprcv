#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include  "procdef.h"
#include  "glovaldef.h"
#include  "funcdef.h"

void
init_proc(void)
{
  int rtn;

  transinfo.alwble_trnstime.tv_sec = 0;
  transinfo.alwble_trnstime.tv_usec = CALC_PACK_INTVL_USEC(transinfo.trnsrate, transinfo.trnsdtsz);
  printf("Allowable transfer time: %ld.%06ld\n", transinfo.alwble_trnstime.tv_sec, transinfo.alwble_trnstime.tv_usec);

  rcvinfo.sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (rcvinfo.sock < 0)
  {
    fprintf(stderr, "r socket : %s\n", strerror(errno));
    exit(1);
  }
  rtn = bind(rcvinfo.sock, (struct sockaddr *)&rcvinfo.src_addr, sizeof(struct sockaddr_in));
  if (rtn < 0)
  {
    fprintf(stderr, "bind : %s\n", strerror(errno));
    exit(1);
  }
}
