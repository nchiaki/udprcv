#include  <stdio.h>

#include  "procdef.h"
#include  "funcdef.h"
#include  "glovaldef.h"

int
main(int ac, char *av[])
{
  cmdproc(ac, av);

  init_proc();

  gettimeofday(&proc_strttm, NULL);
  while (1)
  {
    recv_proc();
  }

  return 0;
}
