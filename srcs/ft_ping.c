/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:44:01 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/08 17:23:59 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// Calculates the checksum of the whole ICMP packet treating it as a sequence of
// 16-bit words. After the sum it is verified if there is a carry, and if so, it
// is added to the sum (for this reason sum is a 32-bit integer and not 16-bit).
// The final checksum is the 16-bit one's complement of the sum.
// The size of the packet is assumed to be an even number of bytes.
uint16_t	calc_checksum(t_icmp_packet *ptr)
{
	uint32_t	sum;
	uint16_t	*address;
	uint8_t		len;

	sum = 0;
	address = (uint16_t *)ptr;
	len = sizeof(t_icmp_packet);
	while (len > 1)
	{
		sum += *address++;
		len -= sizeof(uint16_t);
	}
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
	return ((uint16_t)(~sum));
}

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
	ping_data->packet.icmp_header.un.echo.sequence++;
}

// Size of icmp_packet when is sent is 64 bytes. When the packet is received its
// size is 84 bytes, since the 20 bytes IP header is now attached at the
// beginning. In any case IP header length is calculated (not assumed to be 20).
// Since recvfrom() is blocking, conditions EWOULDBLOCK / EAGAIN are checked to
// see if the function call failed because no data was available to read within
// the specified timeout period. EINTR is checked to see if the function call
// failed because it was interrupted by a signal, and if so, recvfrom() is
// called again. If correct data is received, the loop is broken and the packet
// info processed and printed, after verification that it is an ICMP_ECHOREPLY
// and the id of the packet is the same as the one sent.
// For ICMP_TIME_EXCEEDED packets, there is no processing since the reference
// ping from inetutils-2.0 is not doing it either. 
void	receive_packet(t_ping_data *ping_data)
{
	struct iphdr	*ip_header;
	char			buff[BUFFER_LEN];
	t_icmp_packet	packet;

	while (1)
	{
		if (recvfrom(ping_data->sockfd, buff, BUFFER_LEN, 0, NULL, NULL) == -1)
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN)
				return ;
			else if (errno == EINTR)
				continue ;
			else
				print_perror_and_exit("recvfrom", ping_data);
		}
		break ;
	}
	ip_header = (struct iphdr *)buff;
	ft_memcpy(&packet, buff + (ip_header->ihl * 4), sizeof(t_icmp_packet));
	if (packet.icmp_header.type == ICMP_TIME_EXCEEDED && \
	ping_data->args.verbose_mode)
		print_ttl_exceeded_line(ping_data, buff, ip_header);
	else if (packet.icmp_header.type == ICMP_ECHOREPLY && \
	packet.icmp_header.un.echo.id == ping_data->packet.icmp_header.un.echo.id)
		print_response_line(ping_data, packet, ip_header->ttl);
}

void	ping_loop(t_ping_data *ping_data)
{
	ping_data->min_time = FLOAT_MAX;
	print_header(ping_data);
	fill_and_send_icmp_packet(ping_data);
	alarm(ping_data->args.interval_seconds);
	while (1)
	{
		receive_packet(ping_data);
		if (ping_data->packet.icmp_header.un.echo.sequence >= \
		ping_data->args.count)
			break ;
	}
	print_summary(ping_data);
	close(ping_data->sockfd);
	return ;
}
