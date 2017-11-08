#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

int timeval_subtract(struct timeval *result, struct timeval *x, 
	struct timeval *y);
void print_partial_result(char sender_ip_strings[][20], 
	struct timeval* czasy, int got, int ttl);
int miliseconds(struct timeval *a);

#endif