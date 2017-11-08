#ifndef ICMP_RECEIVE_ONE_H
#define ICMP_RECEIVE_ONE_H
	
#include <stdint.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#define ICMP_TIME_EXCEEDED 11
#define ICMP_ECHO_REPLAY 0

int	get_single_icmp_packet(int sockfd, 
	char sender_ip_string[][20], struct timeval* czas, 
	struct timeval* remaining, u_int16_t* got_pid, 
	u_int16_t* got_ttl );

#endif