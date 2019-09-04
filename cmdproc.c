#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <libgen.h>
#include <fcntl.h>

#include  "procdef.h"
#include  "glovaldef.h"

static void
usage(char *nm)
{
  char  *iam;
  iam = basename(nm);
  printf("%s rate <transRate> dtsz <dataSize> rsock <IP4addr>:<port> [seqchk]\n", iam);
}

void
cmdproc(int ac, char *av[])
{
  int ix;

  for  (ix=1; ix<ac; ix++)
  {
    if (!strcmp(av[ix], "rate"))
    {
      ix++;
      transinfo.trnsrate = atoi(av[ix]);
    }
    else if (!strcmp(av[ix], "dtsz"))
    {
      ix++;
      transinfo.trnsdtsz = atoi(av[ix]);
    }
    else if (!strcmp(av[ix], "seqchk"))
    {
      seqchkf = 1;
    }
    else if (!strcmp(av[ix], "rsock"))
    {
      char  *v4p, *prtp;
      int prtnm;
      ix++;
      v4p = av[ix];
      prtp = strchr(v4p, ':');
      if (!prtp)
      {
        fprintf(stderr, "ポート番号が指定されていません\n");
        usage(av[0]);
        exit(1);
      }
      *prtp = '\0';
      prtp++;
      prtnm = atoi(prtp);

      rcvinfo.src_addr.sin_family = AF_INET;
      rcvinfo.src_addr.sin_port = htons(prtnm);
      rcvinfo.src_addr.sin_addr.s_addr = inet_addr(v4p);
    }
  }

  if (!transinfo.trnsrate ||
      !transinfo.trnsdtsz ||
      !rcvinfo.src_addr.sin_port)
  {
    if (!transinfo.trnsrate){fprintf(stderr, "rate ");}
    if (!transinfo.trnsdtsz){fprintf(stderr, "dtsz ");}
    if (!rcvinfo.src_addr.sin_port){fprintf(stderr, "rsock ");}
    fprintf(stderr, "の指定をしてください\n");
    usage(av[0]);
    exit(1);
  }
}
