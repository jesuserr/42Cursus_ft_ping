/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:51:08 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/07 15:56:56 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static t_ping_data	*g_static_ping_data = NULL;

void	init_signals(t_ping_data *ping_data)
{
	g_static_ping_data = ping_data;
	if (signal(SIGALRM, signal_handler) == SIG_ERR)
		print_perror_and_exit("SIGALRM signal", NULL);
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		print_perror_and_exit("SIGINT signal", NULL);		
}

// (SIGALRM) When 'count' number of packets transmitted is reached, no more
// packets must be sent, therefore alarm() is not set and SIGALRM is disabled.
// (SIGINT) When the user presses Ctrl+C, summary lines are printed, socket is
// closed and the program exits.
void	signal_handler(int sig)
{
	if (sig == SIGALRM)
	{
		if (g_static_ping_data->packet.icmp_header.un.echo.sequence < \
		g_static_ping_data->args.count)
		{
			fill_and_send_icmp_packet(g_static_ping_data);
			alarm(g_static_ping_data->args.interval_seconds);
		}
	}
	else if (sig == SIGINT)
	{
		print_summary(g_static_ping_data);
		close(g_static_ping_data->sockfd);
		exit(EXIT_SUCCESS);
	}
}
