// Jan Mazur
// 281141

// originally compiled with gcc version 6.2.0 20161005 

// FIX - LAST 3 ECHO PACKETS

#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>

#include "utils.h"
#include "icmp_send.h"
#include "icmp_receive_one.h"

// Nie wiem jak bardzo należy zająć się obsługą błędów więc zostawiam taką flagę.
#define PEDANTIC_ERROR_HANDLING 0

int main(int argc, char *argv[]){

	if(argc != 2){
		fprintf(stderr, "Invalid parameters!\n");
		fprintf(stderr, "Usage: '%s [destination IP]'\n", argv[0] );
		exit(EXIT_FAILURE);
	}

	struct in_addr recipient;
	
	if(inet_aton(argv[1],&recipient) == 0){
		fprintf(stderr, "Wrong IP address!\n");
		fprintf(stderr, "Usage: %s x.x.x.x",argv[0]);
		exit(EXIT_FAILURE);
	}

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if(sockfd < 0){
		fprintf(stderr, "socket error: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	int pid = getpid();
	int end_flag = 0;

	for(int ttl=1; ttl<=30; ttl++){
		// sending 3 packets
		for(int j=0; j<3; j++){
			//                                        v sequence = ttl
			if(icmp_send(sockfd, pid, ttl, &recipient,ttl ) < 0){
				printf("error sending packet\n");
				exit(EXIT_FAILURE);
			}
		}
		
		// getting replays
		struct timeval start, end, partial, remaining;
		gettimeofday(&start,NULL);
		end.tv_sec = start.tv_sec + 1;
		end.tv_usec = start.tv_usec;
		partial.tv_sec = start.tv_sec;
		partial.tv_usec = start.tv_usec;

		int got = 0;
		char sender_ip_string[3][20];
		struct timeval czasy[3];

		// int got_echo = 0;
		// int echo_count = 0;
		while( timeval_subtract(&remaining,&end,&partial) == 0 /*&&  end_flag == 0*/){
			u_int16_t got_pid;
			u_int16_t got_ttl;
			int result = get_single_icmp_packet(sockfd, &(sender_ip_string[got]), 
				&(czasy[got]), &remaining, &got_pid, &got_ttl );

			//time exceeded
			if(result == 1){
				if( got_pid == pid && got_ttl == ttl){
					got++;
					if(got == 3) break;
				}				
			}

			// echo_replay
			if(result == 2){
				if( got_pid == pid && got_ttl == ttl){
					got++;
					// got_echo++;
					// echo_count++;
					end_flag = 1;
					if(got == 3 /*&& got_echo == 3*/){
						printf("got %d echo\n",got);
						break;
					} 
				}
			}

#if PEDANTIC_ERROR_HANDLING

			// nothing at all in demanding time
			if(result == -3){
				break;
			}

			// recvfrom error. may still proceed
			if(result == -1){
				printf("revcv error\n");
			}

			// icmp but not wanted type
			if(result == 0){
				
			}

			//may still proceed
			if(result == -2){
				printf("select error\n");
			}
#endif
			gettimeofday(&partial,NULL);
		}	// endwhile

		print_partial_result(sender_ip_string,czasy, got, ttl);

		// echo_replay
		if(end_flag == 1){
			// printf("success");
			exit(EXIT_SUCCESS);
		}
	}	

	// more than 30 ttl away
	exit(EXIT_SUCCESS);
}
