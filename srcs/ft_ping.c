/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:44:01 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/04 13:28:03 by jesuserr         ###   ########.fr       */
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
	ping_data->packet.icmp_header.checksum = calc_checksum(&ping_data->packet);
	bytes_sent = sendto(ping_data->sockfd, &ping_data->packet, \
	sizeof(ping_data->packet), 0, (struct sockaddr *)&ping_data->dest_addr, \
	sizeof(ping_data->dest_addr));
	if (bytes_sent == -1)
		print_perror_and_exit("sendto", ping_data);
}

void	read_loop(t_ping_data *ping_data)
{
	while (1)
	{
		fill_and_send_icmp_packet(ping_data);
		ping_data->packet.icmp_header.un.echo.sequence++;
		if (ping_data->args.stop_after_count && \
		ping_data->packet.icmp_header.un.echo.sequence >= ping_data->args.count)
			break ;
		sleep(ping_data->args.interval_seconds);
	}
	return ;
}
