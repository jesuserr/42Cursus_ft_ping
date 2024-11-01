/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:44:01 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/01 23:03:47 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	send_ping(t_ping_data *ping_data)
{
	ssize_t	bytes_sent;

	ping_data->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ping_data->sockfd == -1)
		print_perror_and_exit("socket");
	printf("Socket created\n");
	bytes_sent = sendto(ping_data->sockfd, &ping_data->packet, \
		sizeof(ping_data->packet), 0, (struct sockaddr *)&ping_data->dest_addr, \
		sizeof(ping_data->dest_addr));
	if (bytes_sent == -1)
		print_perror_and_exit("sendto");
	printf("Packet sent\n");
	return ;
}
