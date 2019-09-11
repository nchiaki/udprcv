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

  if (PCKDAT_MAXZ < rcvinfo.rcv_actlen)
  {
    fprintf(stderr, "\r%s\r%ld.%06ld:Rcv BigData Rcv %d\n", spsln, tmptm.tv_sec, tmptm.tv_usec, rcvinfo.rcv_actlen);
    return;
  }

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
    printf("\r%s\r%ld.%06ld:Rcv %d.%d Mbps %ld,%06ld %d/%d", spsln, tmptm.tv_sec, tmptm.tv_usec, mrate, krate, intrvltm.tv_sec, intrvltm.tv_usec, sqchers, sqchks);
    fflush(stdout);
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
          char  cnmzros[32],errrtstr[64];
          int   errrt, errcnt, dx;
          timersub(&rcvinfo.rcv_time, &proc_strttm, &tmptm);
          sqchers++;

          dx = 0;
          errcnt = sqchers * 100;
          errrt = errcnt / sqchks;
          if (errrt)
          {
            sprintf(errrtstr, "%d%%", errrt);
          }
          else
          {
            cnmzros[dx++] = '0';
            cnmzros[dx] = '\0';
            errcnt *= 10;
            errrt = errcnt / sqchks;
            while (!errrt)
            {
              cnmzros[dx++] = '0';
              cnmzros[dx] = '\0';
              errcnt *= 10;
              errrt = errcnt / sqchks;
            }
            sprintf(errrtstr, "0.%s%d%%", cnmzros, errrt);
          }
          printf("\n%ld.%06ld:RD SQER %d -> %d %ld.%06ld %d/%d %s\n", tmptm.tv_sec, tmptm.tv_usec, pre_rcv_data_sqno, sqno, intrvltm.tv_sec, intrvltm.tv_usec, sqchers, sqchks, errrtstr);
        }
      }
    }
    pre_rcv_data_sqno =  sqno;
  }
}
