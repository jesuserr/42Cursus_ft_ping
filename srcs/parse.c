/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:41:38 by jesuserr          #+#    #+#             */
/*   Updated: 2024/10/31 20:26:19 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "srcs/ft_ping.h"

void	print_usage(void)
{
	printf("Usage\n"
		"  ft_ping [options] <destination>\n\n"
		"Options:\n"
		"  <destination>      dns name or ip address\n"
		"  -a                 use audible ping\n"
		"  -A                 use adaptive ping\n"
		"  -B                 sticky source address\n"
		"  -c <count>         stop after <count> replies\n"
		"  -D                 print timestamps\n"
		"  -d                 use SO_DEBUG socket option\n"
		"  -f                 flood ping\n"
		"  -h or -?           print help and exit\n"
		"  -I <interface>     either interface name or address\n"
		"  -i <interval>      seconds between sending each packet\n"
		"  -L                 suppress loopback of multicast packets\n"
		"  -l <preload>       send <preload> number of packages while waiting replies\n"
		"  -m <mark>          tag the packets going out\n"
		"  -M <pmtud opt>     define mtu discovery, can be one of <do|dont|want>\n"
		"  -n                 no dns name resolution\n"
		"  -O                 report outstanding replies\n"
		"  -p <pattern>       contents of padding byte\n"
		"  -q                 quiet output\n"
		"  -Q <tclass>        use quality of service <tclass> bits\n"
		"  -s <size>          use <size> as number of data bytes to be sent\n"
		"  -S <size>          use <size> as SO_SNDBUF socket option value\n"
		"  -t <ttl>           define time to live\n"
		"  -U                 print user-to-user latency\n"
		"  -v                 verbose output\n"
		"  -V                 print version and exit\n"
		"  -w <deadline>      reply wait <deadline> in seconds\n"
		"  -W <timeout>       time to wait for response\n\n"
		"IPv4 options:\n"
		"  -4                 use IPv4\n"
		"  -b                 allow pinging broadcast\n"
		"  -R                 record route\n"
		"  -T <timestamp>     define timestamp, can be one of <tsonly|tsandaddr|tsprespec>\n");
	return ;
}

void	print_error(void)
{
	printf("ft_ping: usage error: Destination address required\n");
	printf("Try 'ft_ping -h' or 'ft_ping -?' for more information.\n");
	exit (EXIT_FAILURE);
}

void	parse_arguments(int argc, char **argv, t_arguments *args)
{
	int		opt;

	if (argc < 2)
		print_error();
	while ((opt = getopt(argc, argv, ":hvVD")) != -1)
	{
		switch (opt)
		{
			case 'V':
				printf("ft_ping version 1.0 based on ping implementation from");
				printf(" inetutils 2.0\n");
				exit(EXIT_SUCCESS);
			case '?':
			case 'h':
				print_usage();
				exit(EXIT_SUCCESS);
			case 'v':
				args->verbose_mode = true;
				break ;
			case 'D':
				args->print_timestamps = true;
				break ;
			default :
				print_error();
		}
	}
	if (optind >= argc)
		print_error();
	args->destination = argv[optind];
	return ;
}
