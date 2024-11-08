/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:41:38 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/08 17:48:20 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// Mandatory: -h or -?, -v and -V (3)
// Bonus: -c <count>, -D, -i <interval>, -q, -t <ttl> and -W <timeout> (6)
void	print_usage(void)
{
	printf("Usage\n"
		"  ./ft_ping [options] <destination>\n\n"
		"Options:\n"
		"  <destination>      dns name or ip address\n"
		"  -c <count>         stop after <count> replies\n"
		"  -D                 print timestamps\n"
		"  -h or -?           print help and exit\n"
		"  -i <interval>      seconds between sending each packet\n"
		"  -q                 quiet output\n"
		"  -t <ttl>           define time to live\n"
		"  -v                 verbose output\n"
		"  -V                 print version and exit\n"
		"  -W <timeout>       time to wait for response\n");
	return ;
}

bool	check_if_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

u_int8_t	check_argument_value_ttl(char *optarg)
{
	int32_t	value;

	value = ft_atoi(optarg);
	if (ft_strlen(optarg) > 3 || !check_if_only_digits(optarg) || value == 0 || \
	value > 255)
		print_error_and_exit("Out of range: 0 < integer value < 255");
	return ((u_int8_t)value);
}

int32_t	check_argument_value(char *optarg)
{
	int32_t	value;

	value = ft_atoi(optarg);
	if (ft_strlen(optarg) > 9 || !check_if_only_digits(optarg) || value == 0)
		print_error_and_exit("Out of range: 0 < integer value < 999999999");
	return (value);
}

void	parse_arguments(int argc, char **argv, t_arguments *args)
{
	int		opt;

	args->interval_seconds = DEFAULT_INTERVAL;
	args->count = INT32_MAX;
	args->ttl = DEFAULT_TTL;
	args->timeout = DEFAULT_TIMEOUT;
	while ((opt = getopt(argc, argv, "c:Dh?i:qt:vVW:")) != -1)
	{
		switch (opt)
		{
			case 'c':
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
			case 'q':
				args->quiet_mode = true;
				break ;
			case 't':
				args->ttl = check_argument_value_ttl(optarg);
				break ;
			case 'v':
				args->verbose_mode = true;
				break ;
			case 'V':
				printf("ft_ping 1.0 based on ping build from inetutils 2.0\n");
				exit(EXIT_SUCCESS);
			case 'W':
				args->timeout = check_argument_value(optarg);
				break ;
		}
	}
	if (optind >= argc)
		print_error_and_exit("Destination address required");
	if (getuid() != 0)
		print_error_and_exit("Superuser privileges needed to run the program.");
	args->dest = argv[optind];
	return ;
}
