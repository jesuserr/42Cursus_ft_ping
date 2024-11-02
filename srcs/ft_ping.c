/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:44:01 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/02 21:18:46 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// is really needed sockfd to be inside t_ping_data?
void	send_ping(t_ping_data *ping_data)
{
	ssize_t	bytes_sent;

	ping_data->sockfd = socket(SOCKET_DOMAIN, SOCKET_TYPE, SOCKET_PROTOCOL);
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
