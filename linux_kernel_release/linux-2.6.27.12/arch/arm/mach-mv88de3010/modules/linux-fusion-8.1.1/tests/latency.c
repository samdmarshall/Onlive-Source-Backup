/*
 *      Fusion Kernel Module
 *
 *      (c) Copyright 2002  Convergence GmbH
 *
 *      Written by Denis Oliver Kropp <dok@directfb.org>
 *
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/ioctl.h>

#include <linux/fusion.h>

#include <pthread.h>

typedef struct {
  int            nr;
  struct timeval stamp;
} TestMessage;

static int       fd;       /* File descriptor of the Fusion Kernel Device */
static pthread_t receiver; /* Thread reading messages from the device. */

static void
process_sent_message (int msg_id, int msg_size, void *msg_data)
{
  long            d;
  struct timeval  t;
  TestMessage    *message = (TestMessage*) msg_data;

  /* Stop time of arrival. */
  gettimeofday (&t, NULL);

  /* Calculate latency. */
  d = ((t.tv_sec - message->stamp.tv_sec) * 1000000 +
       (t.tv_usec - message->stamp.tv_usec));

  /* Print message. */
  printf ("Got message with %ld.%03ld ms latency.\n",
          d / 1000, d % 1000);
}

static void
process_reactor_message (int reactor_id, int msg_size, void *msg_data)
{
  /* This program doesn't make use of reactors. */
}

static void *
receiver_thread (void *arg)
{
  int  len;
  char buf[1024];

  /* Read as many messages as possible at once. */
  while ((len = read (fd, buf, 1024)) > 0 || errno == EINTR)
    {
      /* Current position within the buffer. */
      char *buf_p = buf;

      /* Shutdown? */
      pthread_testcancel();

      /* Possibly interrupted during blocking read. */
      if (len <= 0)
        continue;

      /* While there are still messages in the buffer... */
      while (buf_p < buf + len)
        {
          /* Header of next message is at the current buffer location. */
          FusionReadMessage *header = (FusionReadMessage*) buf_p;

          /* Its data follows immediately. */
          void *data = buf_p + sizeof(FusionReadMessage);

          /* Process the message depending on its type (origin). */
          switch (header->msg_type)
            {
            case FMT_SEND:
              process_sent_message (header->msg_id,
                                    header->msg_size, data);
              break;
            case FMT_REACTOR:
              process_reactor_message (header->msg_id,
                                       header->msg_size, data);
              break;
            default:
              break;
            }

          /* Shutdown? */
          pthread_testcancel();

          /* Next message header follows immediately. */
          buf_p = data + header->msg_size;
        }
    }

  perror ("receiver thread failure");

  return NULL;
}

int
main (int argc, char *argv[])
{
  int n;
  FusionEnter enter = {{ FUSION_API_MAJOR, FUSION_API_MINOR }};

  /* Open the Fusion Kernel Device. */
  fd = open ("/dev/fusion/0", O_RDWR);
  if (fd < 0)
    {
      perror ("opening /dev/fusion failed");
      return -1;
    }

  /* Query our fusion id. */
  if (ioctl (fd, FUSION_ENTER, &enter))
    {
      perror ("FUSION_ENTER failed");
      close (fd);
      return -2;
    }

  /* Start the receiver thread. */
  pthread_create (&receiver, NULL, receiver_thread, NULL);

  /* Wait for the receiver being up. */
  usleep (100000);

  /* Send some messages. */
  for (n = 0; n < 10; n++)
    {
      FusionSendMessage send;           /* Message header. */
      TestMessage       message;        /* Message data. */

      /* Fill message header. */
      send.fusion_id = enter.fusion_id; /* recipient */
      send.msg_id    = 0;               /* optional */
      send.msg_size  = sizeof(TestMessage);
      send.msg_data  = &message;

      /* Set message number. */
      message.nr = n;

      /* Stop time of departure. */
      gettimeofday (&message.stamp, NULL);

      /* Post the message. */
      if (ioctl (fd, FUSION_SEND_MESSAGE, &send))
        perror ("FUSION_SEND_MESSAGE failed");

      /* Wait some random time. */
      usleep (rand()%100000 + 100000);
    }

  /* Stop the receiver. */
  pthread_cancel (receiver);
  pthread_join (receiver, NULL);

  /* Close the Fusion Kernel Device. */
  close (fd);

  return 0;
}
