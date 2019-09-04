#include  "funcdef.h"

int
main(int ac, char *av[])
{
  cmdproc(ac, av);

  init_proc();

  while (1)
  {
    recv_proc();
  }

  return 0;
}
