/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:43:53 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/12 12:14:58 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/
# include "../libft/includes/libft.h"
# include <stdio.h>				// for printf
# include <stdlib.h>			// for exit
# include <unistd.h>			// for getopt
# include <stdbool.h>			// for booleans
# include <bits/getopt_core.h>	// Delete, just to fix intellisense vscode error
# include <sys/socket.h>		// for socket, sendto
# include <netinet/in.h>		// for IPPROTO_ICMP
# include <netinet/ip_icmp.h>	// for struct icmphdr
# include <netinet/ip.h>		// for struct iphdr
# include <arpa/inet.h>			// for inet_addr
# include <sys/time.h>  		// for gettimeofday
# include <sys/types.h>			// for struct addrinfo
# include <netdb.h>				// for struct addrinfo
# include <signal.h>			// for signals (SIGALRM, SIGINT)
# include <math.h>				// for sqrt
# include <errno.h>				// for errno

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define SOCKET_DOMAIN		AF_INET
# define SOCKET_TYPE		SOCK_RAW
# define SOCKET_PROTOCOL	IPPROTO_ICMP
# define ICMP_PACKET_SIZE	64
# define PAYLOAD_40_B		"Written by Jesus Serrano on November '24"
# define BUFFER_LEN			1024
# define FLOAT_MAX			3.402823466e+38F
# define DEFAULT_INTERVAL	1					// seconds
# define DEFAULT_TTL		64					// hops
# define DEFAULT_TIMEOUT	10					// seconds

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_arguments
{
	char		*dest;
	bool		verbose_mode;
	bool		print_timestamps;
	bool		interval;
	bool		quiet_mode;
	int32_t		count;
	int32_t		interval_seconds;
	int32_t		timeout;
	u_int8_t	ttl;
}	t_arguments;

typedef struct s_icmp_packet
{
	struct icmphdr	icmp_header;
	uint64_t		seconds;
	uint64_t		microseconds;
	char			payload[ICMP_PACKET_SIZE - sizeof(struct icmphdr) - \
					2 * sizeof(uint64_t)];
}	t_icmp_packet;

typedef struct s_timings
{
	float				min_time;
	float				max_time;
	float				sum_times;
	float				mean_time;
	float				square_dist;
}	t_timings;

typedef struct s_ping_data
{
	t_icmp_packet		packet;
	t_arguments			args;
	struct sockaddr_in	dest_addr;
	struct addrinfo		hints;
	int					sockfd;
	char				ip_str[INET_ADDRSTRLEN];
	uint16_t			packets_received;
	uint16_t			ttl_packets_received;
	t_timings			timings;
}	t_ping_data;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/
/********************************** ft_ping.c *********************************/
const char	*turn_ip_to_str(t_ping_data *ping_data, void *src, char *dst);
void		fill_and_send_icmp_packet(t_ping_data *ping_data);
void		ping_loop(t_ping_data *ping_data);

/********************************** main.c ************************************/
void		print_usage(void);

/********************************** messages.c ********************************/
void		print_header(t_ping_data *ping_data);
void		print_response_line(t_ping_data *ping_data, t_icmp_packet packet, \
			uint8_t ttl);
void		print_ttl_exceeded_line(t_ping_data *ping_data, char *buff, \
			struct iphdr *ip_header);
void		print_summary(t_ping_data *ping_data);

/********************************** parser.c **********************************/
void		parse_arguments(int argc, char **argv, t_arguments *args);

/********************************** signals.c *********************************/
void		init_signals(t_ping_data *ping_data);
void		signal_handler(int sig);
void		print_error_and_exit(char *str);
void		print_perror_and_exit(char *msg, t_ping_data *ping_data);

#endif
