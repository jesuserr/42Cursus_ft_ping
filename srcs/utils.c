/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 22:41:39 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/03 17:46:52 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// Calculates the checksum of the whole ICMP packet treating it as a sequence of
// 16-bit words. After the sum, it is verified if there is a carry and if so it
// is added to the sum (for this reason sum is a 32-bit integer and not 16-bit).
// The final checksum is the 16-bit one's complement of the sum. 
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

void	print_perror_and_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

bool	check_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void	signal_handler(int sig)
{
	if (sig == SIGALRM)
		printf("Signal alarm received\n");
	else if (sig == SIGINT)
	{
		printf("Signal int received\n");
		exit(EXIT_SUCCESS);
	}
}
