#include  <stdio.h>
#include  "procdef.h"
#include  "glovaldef.h"

unsigned short int  pre_rcv_data_sqno;
int   sqchks = 0;
int   sqchers = 0;

struct timeval  pre_rcv_tm = {0};
struct timeval  lst1_rcv_tm = {0};
struct timeval  unit_tm = {.tv_sec=1, .tv_usec=0};

long  unit_ttlrcvs = 0;
long long intrvl_ttl_usec = 0;
int       intrvl_cnt = 0;
void
recv_proc(void)
{
  struct timeval  tmptm, intrvltm;;
  int mrate, krate;

  do {
    rcvinfo.rcv_actlen = recv(rcvinfo.sock, rcvinfo.rcv_data, RCVDTARAZ, MSG_DONTWAIT);
  } while (rcvinfo.rcv_actlen <= 0);
  unit_ttlrcvs += rcvinfo.rcv_actlen;

  gettimeofday(&rcvinfo.rcv_time, NULL);
  timersub(&rcvinfo.rcv_time, &lst1_rcv_tm, &intrvltm);
  timersub(&rcvinfo.rcv_time, &pre_rcv_tm, &tmptm);
  if (timercmp(&unit_tm, &tmptm, <=))
  {
    timersub(&rcvinfo.rcv_time, &proc_strttm, &tmptm);

    intrvl_ttl_usec /= intrvl_cnt;
    intrvl_cnt = 0;
    intrvltm.tv_sec = intrvl_ttl_usec / 1000000;
    intrvltm.tv_usec = intrvl_ttl_usec % 1000000;

    unit_ttlrcvs *= 8;
    mrate = unit_ttlrcvs / 1000000;
    krate = (unit_ttlrcvs % 1000000) / 1000;
    printf("%ld.%06ld:Rcv %d.%d Mbps %ld,%06ld %d/%d\n", tmptm.tv_sec, tmptm.tv_usec, mrate, krate, intrvltm.tv_sec, intrvltm.tv_usec, sqchers, sqchks);
    unit_ttlrcvs = 0;
    pre_rcv_tm = rcvinfo.rcv_time;

    intrvltm.tv_sec = intrvltm.tv_usec = 0;
  }
  intrvl_ttl_usec += ((intrvltm.tv_sec * 1000000)+intrvltm.tv_usec);
  intrvl_cnt++;
  lst1_rcv_tm = rcvinfo.rcv_time;

  if (seqchkf)
  {
    unsigned short int  sqno;

    sqno = (rcvinfo.rcv_data[2] << 8)|rcvinfo.rcv_data[3];

    sqchks++;
    if ((pre_rcv_data_sqno+1) != sqno)
    {
      if (1 < sqchks)
      {
        if (!(((unsigned short int)pre_rcv_data_sqno == 65535) && ((unsigned short int)sqno == 0)))
        {
          timersub(&rcvinfo.rcv_time, &proc_strttm, &tmptm);
          sqchers++;
          printf("%ld.%06ld:RD SQER %d/%d %d -> %d %ld.%06ld\n", tmptm.tv_sec, tmptm.tv_usec, sqchers, sqchks, pre_rcv_data_sqno, sqno, intrvltm.tv_sec, intrvltm.tv_usec);
        }
      }
    }
    pre_rcv_data_sqno =  sqno;
  }
}
