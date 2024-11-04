/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:41:38 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/04 16:28:43 by jesuserr         ###   ########.fr       */
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
		"  -i <interval>      seconds between sending each packet\n"
		"  -v                 verbose output\n"
		"  -V                 print version and exit\n");
	return ;
}

int32_t	check_argument_value(char *optarg)
{
	int32_t	value;

	value = ft_atoi(optarg);
	if (ft_strlen(optarg) > 9 || !check_only_digits(optarg) || value == 0)
		print_error_and_exit("Out of range: 0 < value < 999999999");
	return (value);
}

void	parse_arguments(int argc, char **argv, t_arguments *args)
{
	int		opt;
	
	args->interval_seconds = 1;
	while ((opt = getopt(argc, argv, "c:Dh?i:vV")) != -1)
	{
		switch (opt)
		{
			case 'c':
				args->stop_after_count = true;
				args->count = check_argument_value(optarg);
				break ;
			case 'D':
				args->print_timestamps = true;
				break ;
			case 'h':
			case '?':
				print_usage();
				exit(EXIT_SUCCESS);
			case 'i':
				args->interval = true;
				args->interval_seconds = check_argument_value(optarg);
				break ;
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
	if (getuid() != 0)
		print_error_and_exit("Superuser privileges needed to run the program.");
	args->dest = argv[optind];
	return ;
}
