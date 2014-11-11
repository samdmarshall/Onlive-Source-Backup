//application sender: netlink-exam-user-send.c

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


int main(int argc, char * argv[])
{
  struct sockaddr_nl saddr, daddr;
  struct nlmsghdr *nlhdr = NULL;
  struct msghdr msg;
  struct iovec iov;
  int sd;
  int ret = -1;

  UCHAR MsgBuf[MV_CC_ICCFIFO_FRAME_SIZE];
  UCHAR *pMsgBuf = MsgBuf;

  sd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GALOIS_CC_NEW);

  printf("create a socket (%d).\n", sd);
    
  memset(&saddr, 0, sizeof(saddr));
  memset(&daddr, 0, sizeof(daddr));

  saddr.nl_family = AF_NETLINK;      
//  saddr.nl_pid = NETLINK_GALOIS_CC_PID_SEND;
//  saddr.nl_groups = NETLINK_GALOIS_CC_GROUP;
  
  saddr.nl_pid = getpid();
  saddr.nl_groups = 0;  
  
  ret = bind(sd, (struct sockaddr*)&saddr, sizeof(saddr));

  printf("bind the socket [%d].\n", ret);

  if (ret == -1)
  	exit(0);

  daddr.nl_family = AF_NETLINK;
  daddr.nl_pid = NETLINK_GALOIS_CC_PID_KERN;
  daddr.nl_groups = 0;

  nlhdr = (struct nlmsghdr *)malloc(NLMSG_SPACE(MV_CC_ICCFIFO_FRAME_SIZE));

	/* assemble ICCP Datagram */
	ICCP_SrcCPU(pMsgBuf) = 0;
	ICCP_SrcProcess(pMsgBuf) = MV_OSAL_PROCESS_DEFAULT;
	
	ICCP_DstCPU(pMsgBuf) = 0;
	ICCP_DstProcess(pMsgBuf) = MV_OSAL_PROCESS_DEFAULT;
	
	ICCP_DatagramID(pMsgBuf) = 0;
	ICCP_Protocol(pMsgBuf) = MV_CC_Protocol_CCUDP;
	ICCP_Padding(pMsgBuf) = 0xFF;
	ICCP_TimeStamp(pMsgBuf) = 0x12345678;	

  while (1)
  {
	ICCP_Padding(pMsgBuf)++;

    memset(&msg, 0 ,sizeof(struct msghdr));
    memcpy(NLMSG_DATA(nlhdr), pMsgBuf, MV_CC_ICCFIFO_FRAME_SIZE);
    
    nlhdr->nlmsg_len = NLMSG_LENGTH(MV_CC_ICCFIFO_FRAME_SIZE);
    nlhdr->nlmsg_pid = NETLINK_GALOIS_CC_PID_SEND;  /* self pid */
    nlhdr->nlmsg_flags = 0;

    iov.iov_base = (void *)nlhdr;
    iov.iov_len = nlhdr->nlmsg_len;
    
    msg.msg_name = (void *)&daddr;
    msg.msg_namelen = sizeof(daddr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    
    ret = sendmsg(sd, &msg, 0);
    if (ret == -1) {
      perror("sendmsg error");
      exit(0);
    }

	TestFramePrint(pMsgBuf, MV_CC_ICCFIFO_FRAME_SIZE);
	
	sleep(1);
  }
 
  close(sd);
}
