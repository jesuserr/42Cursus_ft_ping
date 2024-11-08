/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 22:41:39 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/07 20:53:42 by jesuserr         ###   ########.fr       */
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

void	print_error_and_exit(char *str)
{
	printf("ft_ping: usage error: %s\n", str);
	printf("Try 'ft_ping -h' or 'ft_ping -?' for more information.\n");
	exit (EXIT_FAILURE);
}

// Prints system error message, closes the socket if ping_data has been passed 
// and the socket is open and then exits with EXIT_FAILURE status.
void	print_perror_and_exit(char *msg, t_ping_data *ping_data)
{
	perror(msg);
	if (ping_data && ping_data->sockfd > 0)
		close(ping_data->sockfd);
	exit(EXIT_FAILURE);
}
