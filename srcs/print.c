/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:31:42 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/04 19:46:47 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_header(t_ping_data *ping_data)
{
	char	ip_str[INET_ADDRSTRLEN];

	if (inet_ntop(AF_INET, &(ping_data->dest_addr.sin_addr), ip_str, \
	INET_ADDRSTRLEN) == NULL)
		print_perror_and_exit("inet_ntop", ping_data);
	printf("PING %s (%s): %ld data bytes\n", ping_data->args.dest, ip_str, \
	sizeof(t_icmp_packet));
}
