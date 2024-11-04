/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:06:52 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/04 13:28:14 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	init_signals(void)
{
	if (signal(SIGALRM, signal_handler) == SIG_ERR)
		print_perror_and_exit("signal", NULL);
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		print_perror_and_exit("signal", NULL);
}

void	init_ping_data_and_socket(t_ping_data *ping_data)
{
	struct addrinfo	*result;
	int				ret;

	ping_data->hints.ai_family = SOCKET_DOMAIN;
	ping_data->hints.ai_socktype = SOCKET_TYPE;
	ping_data->hints.ai_protocol = SOCKET_PROTOCOL;
	ret = getaddrinfo(ping_data->args.dest, NULL, &ping_data->hints, &result);
	if (ret != 0)
	{
		fprintf(stderr, "ft_ping: %s: %s\n", ping_data->args.dest, \
			gai_strerror(ret));
		exit(EXIT_FAILURE);
	}
	ping_data->dest_addr.sin_family = SOCKET_DOMAIN;
	ping_data->dest_addr.sin_addr = ((struct sockaddr_in *)result->ai_addr) \
		->sin_addr;
	freeaddrinfo(result);
	ping_data->packet.icmp_header.type = ICMP_ECHO;
	ping_data->packet.icmp_header.code = 0;
	ping_data->packet.icmp_header.un.echo.id = getpid() & 0xFFFF;
	ping_data->packet.icmp_header.un.echo.sequence = 0;
	ft_memcpy(ping_data->packet.payload, PAYLOAD_40_B, sizeof(PAYLOAD_40_B));
	ping_data->sockfd = socket(SOCKET_DOMAIN, SOCKET_TYPE, SOCKET_PROTOCOL);
	if (ping_data->sockfd == -1)
		print_perror_and_exit("socket", ping_data);
}

void	print_debug_data(t_ping_data *ping_data)
{
	printf("Destination: %s\n", ping_data->args.dest);
	printf("Verbose mode: %d\n", ping_data->args.verbose_mode);
	printf("Print timestamps: %d\n", ping_data->args.print_timestamps);
	printf("Stop after count: %d\n", ping_data->args.stop_after_count);
	printf("Count: %d\n", ping_data->args.count);
	ft_hex_dump(ping_data, sizeof(t_ping_data), 16);
	printf("size of t_ping_data: %ld\n", sizeof(t_ping_data));
}

int	main(int argc, char **argv)
{
	t_ping_data	ping_data;

	if (argc < 2)
		print_error_and_exit("Destination address required");
	ft_bzero(&ping_data, sizeof(t_ping_data));
	parse_arguments(argc, argv, &ping_data.args);
	init_signals();
	init_ping_data_and_socket(&ping_data);
	read_loop(&ping_data);
	print_debug_data(&ping_data);
	close(ping_data.sockfd);
	return (0);
}
