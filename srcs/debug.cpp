#include <arpa/inet.h>
#include <cstdio>

void	accepted_connection_from(const char *sin_addr)
{
	char	ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, sin_addr, &ip[0], INET_ADDRSTRLEN);
	printf("connection accepted from %s\n", &ip[0]);
}