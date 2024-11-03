/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:41:38 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/03 19:54:43 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_usage(void)
{
	printf("Usage\n"
		"  ft_ping [options] <destination>\n\n"
		"Options:\n"
		"  <destination>      dns name or ip address\n"
		"  -c <count>         stop after <count> replies\n"
		"  -D                 print timestamps\n"
		"  -h or -?           print help and exit\n"
		"  -v                 verbose output\n"
		"  -V                 print version and exit\n");
	return ;
}

void	parse_arguments(int argc, char **argv, t_arguments *args)
{
	int		opt;

	while ((opt = getopt(argc, argv, "c:Dh?vV")) != -1)
	{
		switch (opt)
		{
			case 'c':
				args->stop_after_count = true;
				args->count = ft_atoi(optarg);
				if (ft_strlen(optarg) > 9 || check_only_digits(optarg) == false \
					|| args->count == 0)
					print_error_and_exit("out of range: 0 < value < 999999999");
				break ;
			case 'D':
				args->print_timestamps = true;
				break ;
			case 'h':
			case '?':
				print_usage();
				exit(EXIT_SUCCESS);
			case 'v':
				args->verbose_mode = true;
				break ;
			case 'V':
				printf("ft_ping 1.0 based on ping build from inetutils 2.0\n");
				exit(EXIT_SUCCESS);		
		}
	}
	if (optind >= argc)
		print_error_and_exit("Destination address required");
	args->dest = argv[optind];
	return ;
}
