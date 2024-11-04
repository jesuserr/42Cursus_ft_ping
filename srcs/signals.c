/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:51:08 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/04 19:33:49 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static t_ping_data	*g_static_ping_data = NULL;

void	init_signals(t_ping_data *ping_data)
{
	g_static_ping_data = ping_data;
	if (signal(SIGALRM, signal_handler) == SIG_ERR)
		print_perror_and_exit("signal", NULL);
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		print_perror_and_exit("signal", NULL);
}

void	signal_handler(int sig)
{
	if (sig == SIGALRM)
	{
		fill_and_send_icmp_packet(g_static_ping_data);
		alarm(g_static_ping_data->args.interval_seconds);
	}
	else if (sig == SIGINT)
	{
		printf("Signal int received\n");
		exit(EXIT_SUCCESS);
	}
}
