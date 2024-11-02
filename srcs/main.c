/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:06:52 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/02 22:10:09 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	init_ping_data(t_ping_data *ping_data)
{
	struct timeval	tv;
	struct addrinfo	*result;
	int				ret;

	ping_data->hints.ai_family = SOCKET_DOMAIN;
	ping_data->hints.ai_socktype = SOCKET_TYPE;
	ping_data->hints.ai_protocol = SOCKET_PROTOCOL;
	ret = getaddrinfo(ping_data->args.dest, NULL, &ping_data->hints, &result);
	if (ret != 0)
	{
		fprintf(stderr, "ft_ping: %s: %s\n", ping_data->args.dest, gai_strerror(ret));
		exit(EXIT_FAILURE);
	}
	printf("getaddrinfo success\n");
	ping_data->dest_addr.sin_family = SOCKET_DOMAIN;
	ping_data->dest_addr.sin_addr = ((struct sockaddr_in *)result->ai_addr)->sin_addr;
	freeaddrinfo(result);
	ping_data->packet.icmp_header.type = ICMP_ECHO;
	ping_data->packet.icmp_header.code = 0;
	ping_data->packet.icmp_header.checksum = 0;
	ping_data->packet.icmp_header.un.echo.id = getpid() & 0xFFFF;
	ping_data->packet.icmp_header.un.echo.sequence = 0;
	ft_memcpy(ping_data->packet.payload, PAYLOAD_48_B, sizeof(PAYLOAD_48_B));
	if (gettimeofday(&tv, NULL) == -1)
		print_perror_and_exit("gettimeofday");
	ping_data->packet.time = (uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
	ping_data->packet.icmp_header.checksum = calc_checksum(&ping_data->packet);
}

int	main(int argc, char **argv)
{
	t_ping_data	ping_data;

	ft_bzero(&ping_data, sizeof(t_ping_data));
	if (argc < 2)
		print_error_and_exit();
	parse_arguments(argc, argv, &ping_data.args);
	init_ping_data(&ping_data);
	printf("Destination: %s\n", ping_data.args.dest);
	printf("Verbose mode: %d\n", ping_data.args.verbose_mode);
	printf("Print timestamps: %d\n", ping_data.args.print_timestamps);
	printf("time_sent_ms: %lx\n", ping_data.packet.time);
	send_ping(&ping_data);
	ft_hex_dump(&ping_data, sizeof(t_ping_data), 16);
	printf("size of t_ping_data: %ld\n", sizeof(t_ping_data));
	return (0);
}
