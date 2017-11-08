// Jan Mazur
// 281141

// Function compute_icmp_checksum written by Marcin Bieńkowski.

#include "icmp_send.h"

int icmp_send(int sockfd, int pid, int ttl, 
	struct in_addr *target_address, int sequence_number){

	struct icmphdr icmp_header;

	icmp_header.type = ICMP_ECHO;
	icmp_header.code = 0;
	icmp_header.un.echo.id = pid; //program id
	icmp_header.un.echo.sequence = sequence_number; // setting this the same as ttl when calling icmp_send
	icmp_header.checksum = 0;
	icmp_header.checksum = compute_icmp_checksum (
		(u_int16_t*)&icmp_header, sizeof(icmp_header));


	struct sockaddr_in recipient;
	bzero (&recipient, sizeof(recipient));
	recipient.sin_family = AF_INET;
	recipient.sin_addr = *target_address;

	if( setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)) < 0){
		fprintf(stderr,"setsockopt error: %s\n",strerror(errno));
		return -1;
	}

	ssize_t bytes_sent = sendto (
		sockfd,
		&icmp_header,
		sizeof(icmp_header),
		0,
		(struct sockaddr*)&recipient,
		sizeof(recipient)
	);

	if( bytes_sent < 0){
		printf("błąd przy wysylaniu\n");
		return -1;
	}


	return 0;
}



u_int16_t compute_icmp_checksum (const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}