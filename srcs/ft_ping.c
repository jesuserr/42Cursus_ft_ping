/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:44:01 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/04 19:35:17 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	fill_and_send_icmp_packet(t_ping_data *ping_data)
{
	struct timeval	tv;
	ssize_t			bytes_sent;

	ping_data->packet.icmp_header.checksum = 0;
	if (gettimeofday(&tv, NULL) == -1)
		print_perror_and_exit("gettimeofday", ping_data);
	ping_data->packet.seconds = tv.tv_sec;
	ping_data->packet.microseconds = tv.tv_usec;
	ping_data->packet.icmp_header.un.echo.sequence++;
	ping_data->packet.icmp_header.checksum = calc_checksum(&ping_data->packet);
	bytes_sent = sendto(ping_data->sockfd, &ping_data->packet, \
	sizeof(ping_data->packet), 0, (struct sockaddr *)&ping_data->dest_addr, \
	sizeof(ping_data->dest_addr));
	if (bytes_sent == -1)
		print_perror_and_exit("sendto", ping_data);
}

// Size of icmp_packet when is sent is 64 bytes. When the packet is received its
// size is 84 bytes, since the 20 bytes IP header is now attached at the
// beginning. Instead of assuming that the length of the IP header is always
// 20 bytes, it is actually calculated.
void	receive_packet(t_ping_data *ping_data)
{
	struct iphdr	*ip_header;
	char			buffer[BUFFER_LEN];
	ssize_t			bytes_recv;
	t_icmp_packet	packet;

	bytes_recv = recvfrom(ping_data->sockfd, buffer, BUFFER_LEN, 0, NULL, NULL);
	if (bytes_recv == -1)
		print_perror_and_exit("recvfrom", ping_data);
	ip_header = (struct iphdr *)buffer;
	ft_memcpy(&packet, buffer + (ip_header->ihl * 4), sizeof(t_icmp_packet));
	ft_hex_dump(&packet, sizeof(t_icmp_packet), 16);
	if (packet.icmp_header.un.echo.id == ping_data->packet.icmp_header.un.\
		echo.id)
		printf("Received packet with id %d\n", packet.icmp_header.un.echo.id);
}

void	ping_loop(t_ping_data *ping_data)
{
	print_header(ping_data);
	fill_and_send_icmp_packet(ping_data);
	alarm(ping_data->args.interval_seconds);
	while (1)
	{
		receive_packet(ping_data);
		if (ping_data->args.stop_after_count && \
		ping_data->packet.icmp_header.un.echo.sequence >= ping_data->args.count)
			break ;
	}
	return ;
}
