//application receiver: netlink-exam-user-recv.c

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#include "ICCNet.h"
#include "MV_netlink.h"
#include "testtool.h"

#define BUFFERSIZE 1024

int main(void)
{
  struct sockaddr_nl saddr, daddr;
  struct nlmsghdr *nlhdr = NULL;
  struct msghdr msg;
  struct iovec iov;
  int sd;
  int ret = 1;

	UCHAR MsgBuf[BUFFERSIZE];
	UCHAR *pMsgBuf = MsgBuf;

  sd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GALOIS_CC_NEW);

  printf("create a socket (%d).\n", sd);

  memset(&saddr, 0, sizeof(saddr));
  memset(&daddr, 0, sizeof(daddr));

  saddr.nl_family = AF_NETLINK;      
  saddr.nl_pid = NETLINK_GALOIS_CC_PID_KERN; 
  saddr.nl_groups = NETLINK_GALOIS_CC_GROUP;


  ret = bind(sd, (struct sockaddr*)&saddr, sizeof(saddr));

  printf("bind the socket [%d].\n", ret);
  
  if (ret == -1)
  	exit(0);

  nlhdr = (struct nlmsghdr *)malloc(NLMSG_SPACE(BUFFERSIZE));

  while (1) 
  {
    memset(nlhdr, 0, NLMSG_SPACE(BUFFERSIZE));

    iov.iov_base = (void *)nlhdr;
    iov.iov_len = NLMSG_SPACE(BUFFERSIZE);

    msg.msg_name = (void *)&daddr;
    msg.msg_namelen = sizeof(daddr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    ret = recvmsg(sd, &msg, 0);
/*    printf("ret = %d\n", ret);
    if (ret == 0) 
    {
      printf("Exit.\n");
      exit(0);
    }
    else
    if (ret == -1) 
    {
      perror("recvmsg:");
      exit(1);
    }
*/
    memcpy(pMsgBuf, NLMSG_DATA(nlhdr), MV_CC_ICCFIFO_FRAME_SIZE);
    
    TestFramePrint(pMsgBuf, MV_CC_ICCFIFO_FRAME_SIZE);
  }
 
  close(sd);
}
