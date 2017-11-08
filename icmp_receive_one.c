// Jan Mazur
// 281141

#include "icmp_receive_one.h"
#include <string.h>

int	get_single_icmp_packet(int sockfd,
	char sender_ip_string[][20], struct timeval* czas, 
	struct timeval* remaining, u_int16_t* got_pid, 
	u_int16_t* got_ttl )
{

	struct sockaddr_in 	sender;
	socklen_t 			sender_len = sizeof(sender);
	u_int8_t			buffer[IP_MAXPACKET];

	fd_set descriptors;
	FD_ZERO (&descriptors);
	FD_SET (sockfd, &descriptors);
	struct timeval tv;

	tv.tv_sec = remaining->tv_sec;
	tv.tv_usec = remaining->tv_usec;

	int ready = select(sockfd+1, &descriptors,NULL,NULL, &tv);

	if(ready == 1){

		ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, 
			MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);

		if(packet_len < 0){
			fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
			return -1;
		}

		inet_ntop(AF_INET, &(sender.sin_addr), (char*)sender_ip_string, 20);

		// wyłuskanie danych
		struct iphdr* ip_header = (struct iphdr*) buffer;
		u_int8_t     *icmp_packet = buffer + 4 * ip_header->ihl;
		struct icmphdr* icmp_header = (struct icmphdr*) icmp_packet;

		u_int8_t icmp_type = icmp_header->type;

		if(icmp_type == ICMP_TIME_EXCEEDED){

			struct iphdr* inside_ip_header = (struct iphdr*) ((u_int8_t*)icmp_header + 8);
			u_int8_t * inside_icmp_packet = (u_int8_t*)inside_ip_header + 4 * inside_ip_header->ihl;
			struct icmphdr* inside_icmp_header = (struct icmphdr*) inside_icmp_packet;

			u_int16_t icmp_pid = inside_icmp_header->un.echo.id;
			u_int16_t icmp_sequence = inside_icmp_header->un.echo.sequence;

			*got_ttl = icmp_sequence;
			*got_pid = icmp_pid;

			czas->tv_sec = 0;
			czas->tv_usec = 1000000 - tv.tv_usec;

			return 1;
		}	
		if(icmp_type == ICMP_ECHO_REPLAY){
			u_int16_t icmp_pid = icmp_header->un.echo.id;
			u_int16_t icmp_sequence = icmp_header->un.echo.sequence;

			*got_ttl = icmp_sequence;
			*got_pid = icmp_pid;	

			czas->tv_sec = 0;
			czas->tv_usec = 1000000 - tv.tv_usec;

			return 2;	
		}

		// got ICMP but not echo replay and not time exceeded
		return 0;

	}

	// no icmp in demanging time at all
	if(ready == 0){
		return -3;
	}
	//select fail
	else{
		fprintf(stderr,"select error: %s\n", strerror(errno) );
		return -2;
	}

}

