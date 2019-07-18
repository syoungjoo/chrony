/*
  chronyd/chronyc - Programs for keeping computer clocks accurate.

 **********************************************************************
 * Copyright (C) Miroslav Lichvar  2019
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 **********************************************************************

  =======================================================================

  This is the header file for socket operations.

  */

#ifndef GOT_SOCKET_H
#define GOT_SOCKET_H

#include "addressing.h"

/* Flags for opening sockets */
#define SCK_FLAG_NONBLOCK 1
#define SCK_FLAG_BROADCAST 2
#define SCK_FLAG_RX_DEST_ADDR 4
#define SCK_FLAG_ALL_PERMISSIONS 8
#define SCK_FLAG_PRIV_BIND 16

/* Flags for receiving and sending messages */
#define SCK_FLAG_MSG_ERRQUEUE 1

/* Maximum number of received messages */
#define SCK_MAX_RECV_MESSAGES 4

typedef enum {
  SCK_ADDR_UNSPEC = 0,
  SCK_ADDR_IP,
  SCK_ADDR_UNIX
} SCK_AddressType;

typedef struct {
  void *data;
  unsigned int length;
  SCK_AddressType addr_type;
  int if_index;

  union {
    IPSockAddr ip;
    const char *path;
  } remote_addr;

  union {
    IPAddr ip;
  } local_addr;

  struct {
    struct timespec kernel;
    struct timespec hw;
    int if_index;
    int l2_length;
    int tx_flags;
  } timestamp;
} SCK_Message;

/* Initialisation function */
extern void SCK_Initialise(void);

/* Finalisation function */
extern void SCK_Finalise(void);

/* Get the 0.0.0.0/::0 or 127.0.0.1/::1 address */
extern void SCK_GetAnyLocalIPAddress(int family, IPAddr *local_addr);
extern void SCK_GetLoopbackIPAddress(int family, IPAddr *local_addr);

/* Specify a bind()-like function for binding sockets to privileged ports when
   running in a restricted process (e.g. after dropping root privileges) */
extern void SCK_SetPrivBind(int (*function)(int sock_fd, struct sockaddr *address,
                                            socklen_t address_len));

/* Open socket */
extern int SCK_OpenUdpSocket(IPSockAddr *remote_addr, IPSockAddr *local_addr, int flags);
extern int SCK_OpenTcpSocket(IPSockAddr *remote_addr, IPSockAddr *local_addr, int flags);
extern int SCK_OpenUnixDatagramSocket(const char *remote_addr, const char *local_addr,
                                      int flags);
extern int SCK_OpenUnixStreamSocket(const char *remote_addr, const char *local_addr,
                                    int flags);

/* Set a socket option */
extern int SCK_SetIntOption(int sock_fd, int level, int option, int value);
/* Enable RX timestamping socket option */
extern int SCK_EnableKernelRxTimestamping(int sock_fd);

/* Receive and send data on connected sockets - recv()/send() wrappers */
extern int SCK_Receive(int sock_fd, void *buffer, unsigned int length, int flags);
extern int SCK_Send(int sock_fd, void *buffer, unsigned int length, int flags);

/* Receive a single message or multiple messages.  The functions return the
   number of received messages, or 0 on error.  The returned data point to
   static buffers, which are valid until another call of these functions.  */
extern int SCK_ReceiveMessage(int sock_fd, SCK_Message *message, int flags);
extern int SCK_ReceiveMessages(int sock_fd, SCK_Message *messages, int max_messages,
                               int flags);

/* Initialise a new message (e.g. before sending) */
extern void SCK_InitMessage(SCK_Message *message, SCK_AddressType addr_type);

/* Send a message */
extern int SCK_SendMessage(int sock_fd, SCK_Message *message, int flags);

/* Remove bound Unix socket */
extern int SCK_RemoveSocket(int sock_fd);

/* Close the socket */
extern void SCK_CloseSocket(int sock_fd);

/* Convert between IPSockAddr and sockaddr_in/in6 */
extern void SCK_SockaddrToIPSockAddr(struct sockaddr *sa, int sa_length, IPSockAddr *ip_sa);
extern int SCK_IPSockAddrToSockaddr(IPSockAddr *ip_sa, struct sockaddr *sa, int sa_length);

#endif