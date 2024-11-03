/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:44:01 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/03 23:13:47 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	create_icmp_packet(t_ping_data *ping_data)
{
	struct timeval	tv;

	ping_data->packet.icmp_header.checksum = 0;
	if (gettimeofday(&tv, NULL) == -1)
		print_perror_and_exit("gettimeofday", ping_data);
	ping_data->packet.seconds = tv.tv_sec;
	ping_data->packet.microseconds = tv.tv_usec;
	ping_data->packet.icmp_header.checksum = calc_checksum(&ping_data->packet);
}

void	send_icmp_packet(t_ping_data *ping_data)
{
	ssize_t	bytes_sent;

	ping_data->sockfd = socket(SOCKET_DOMAIN, SOCKET_TYPE, SOCKET_PROTOCOL);
	if (ping_data->sockfd == -1)
		print_perror_and_exit("socket", ping_data);
	printf("Socket created\n");
	while (1)
	{
		create_icmp_packet(ping_data);
		bytes_sent = sendto(ping_data->sockfd, &ping_data->packet, \
		sizeof(ping_data->packet), 0, (struct sockaddr *)&ping_data->dest_addr, \
		sizeof(ping_data->dest_addr));
		if (bytes_sent == -1)
			print_perror_and_exit("sendto", ping_data);
		printf("Packet sent\n");
		sleep(1);
		ping_data->packet.icmp_header.un.echo.sequence++;
		if (ping_data->packet.icmp_header.un.echo.sequence == 5)
			break ;
	}
	return ;
}
