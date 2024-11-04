/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:43:53 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/04 12:50:38 by jesuserr         ###   ########.fr       */
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
# include <arpa/inet.h>			// for inet_addr
# include <sys/time.h>  		// for gettimeofday
# include <sys/types.h>			// for struct addrinfo
# include <netdb.h>				// for struct addrinfo
# include <signal.h>			// for signals (SIGALRM, SIGINT) 

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define SOCKET_DOMAIN		AF_INET
# define SOCKET_TYPE		SOCK_RAW
# define SOCKET_PROTOCOL	IPPROTO_ICMP
# define ICMP_PACKET_SIZE	64
# define PAYLOAD_40_B		"Written by Jesus Serrano on November '24"

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_arguments
{
	char	*dest;
	bool	verbose_mode;
	bool	print_timestamps;
	bool	stop_after_count;
	bool	interval;	
	int32_t	count;
	int32_t	interval_seconds;
}	t_arguments;

typedef struct s_icmp_packet
{
	struct icmphdr	icmp_header;
	uint64_t		seconds;
	uint64_t		microseconds;
	char			payload[ICMP_PACKET_SIZE - sizeof(struct icmphdr) - \
					2 * sizeof(uint64_t)];
}	t_icmp_packet;

typedef struct s_ping_data
{
	t_icmp_packet		packet;
	t_arguments			args;
	struct sockaddr_in	dest_addr;
	struct addrinfo		hints;
	int					sockfd;
}	t_ping_data;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/
/********************************** utils.c ***********************************/
uint16_t	calc_checksum(t_icmp_packet *ptr);
bool		check_only_digits(char *str);
void		print_error_and_exit(char *str);
void		print_perror_and_exit(char *msg, t_ping_data *ping_data);
void		signal_handler(int sig);

/********************************** parse.c ***********************************/
void		parse_arguments(int argc, char **argv, t_arguments *args);

/********************************** ft_ping.c *********************************/
void		fill_and_send_icmp_packet(t_ping_data *ping_data);
void		read_loop(t_ping_data *ping_data);

#endif