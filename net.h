#ifndef NET
#define NET

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "types.h"
#include "logging.h"

/*
 * parse an ipv4 address
 * put the ip in ip
 * convert the port using `atoi` and put it in port
 */
void parse_ip(char host[static 1], string *ip, u32 *port);

/*
 * listen on ip:port
 *
 * return the fd of the server socket
 */
i32 tcp_listen(char host[static 1]);

/*
 * connect to ip:port
 *
 * return the fd of the socket
 */
i32 tcp_connect(char host[static 1]);

/*
 * send data to connfd
 * adding a newline
 *
 * return n bytes sent
 */
usize send_line(i32 connfd, string data);

/*
 * read a line from connfd
 *
 * return string(heap) containing newline
 */
string recv_line(i32 connfd);

#endif
