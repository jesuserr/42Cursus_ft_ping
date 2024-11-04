/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:31:42 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/04 20:44:37 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_header(t_ping_data *ping_data)
{
	if (inet_ntop(AF_INET, &(ping_data->dest_addr.sin_addr), ping_data->ip_str, \
	INET_ADDRSTRLEN) == NULL)
		print_perror_and_exit("inet_ntop", ping_data);
	printf("PING %s (%s): %ld data bytes\n", ping_data->args.dest, \
	ping_data->ip_str, sizeof(t_icmp_packet));
}

void	print_response_line(t_ping_data *ping_data, t_icmp_packet packet, \
		uint8_t ttl)
{
	/*
	printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%ld.%ld ms\n", \
	sizeof(t_icmp_packet), ping_data->args.dest, \
	ping_data->packet.icmp_header.un.echo.sequence, 64, \
	ping_data->packet.seconds, ping_data->packet.microseconds);
	*/
	printf("%ld bytes from %s: ", sizeof(t_icmp_packet), ping_data->ip_str);
	printf("icmp_seq=%d ", packet.icmp_header.un.echo.sequence);
	printf("ttl=%d time=XXX ms \n", ttl);
	// 64 bytes from 79.154.85.235: icmp_seq=0 ttl=165 time=0.914 ms	
}
