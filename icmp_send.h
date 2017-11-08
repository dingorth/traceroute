#ifndef ICMP_SEND_H
#define ICMP_SEND_H

#include <netinet/ip_icmp.h>
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

u_int16_t compute_icmp_checksum (const void *buff, int length);
int icmp_send(int sockfd, int pid, int ttl, 
	struct in_addr *target_address, int sequence_number);

#endif