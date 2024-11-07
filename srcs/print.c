/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:31:42 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/07 09:01:59 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// PING ivoice.synology.me (79.154.85.235): 56 data bytes
void	print_header(t_ping_data *ping_data)
{
	if (inet_ntop(AF_INET, &(ping_data->dest_addr.sin_addr), ping_data->ip_str, \
	INET_ADDRSTRLEN) == NULL)
		print_perror_and_exit("inet_ntop", ping_data);
	printf("PING %s (%s): ", ping_data->args.dest, ping_data->ip_str);
	printf("%ld data bytes\n", sizeof(t_icmp_packet) - sizeof(struct icmphdr));
}

// 64 bytes from 79.154.85.235: icmp_seq=0 ttl=165 time=0.914 ms
// Uses Welford's algorithm to calculate the population standard deviation on
// the fly just in one single pass.
void	print_response_line(t_ping_data *ping_data, t_icmp_packet packet, \
		uint8_t ttl)
{
	struct timeval	tv;	
	float			time_ms;
	float			delta_time;

	ping_data->packets_received++;
	if (gettimeofday(&tv, NULL) == -1)
		print_perror_and_exit("gettimeofday", ping_data);
	tv.tv_sec = tv.tv_sec - packet.seconds;
	tv.tv_usec = tv.tv_usec - packet.microseconds;
	time_ms = tv.tv_sec * 1000 + (float)tv.tv_usec / 1000;
	if (time_ms > ping_data->max_time)
		ping_data->max_time = time_ms;
	if (time_ms < ping_data->min_time)
		ping_data->min_time = time_ms;
	ping_data->sum_times += time_ms;
	delta_time = time_ms - ping_data->mean_time;
	ping_data->mean_time += delta_time / ping_data->packets_received;
	ping_data->square_dist += delta_time * (time_ms - ping_data->mean_time);
	printf("%ld bytes from %s: ", sizeof(t_icmp_packet), ping_data->ip_str);
	printf("icmp_seq=%d ", packet.icmp_header.un.echo.sequence);
	printf("ttl=%d time=%.3f ms \n", ttl, time_ms);
}

// --- ivoice.synology.me ping statistics ---
// 5 packets transmitted, 5 packets received, 0% packet loss
// round-trip min/avg/max/stddev = 0.457/0.886/1.284/0.294 ms
void	print_summary(t_ping_data *ping_data)
{
	float	loss;

	loss = 100 - (((float)ping_data->packets_received / \
		(float)ping_data->packet.icmp_header.un.echo.sequence) * 100);
	printf("--- %s ping statistics ---\n", ping_data->args.dest);
	printf("%d ", ping_data->packet.icmp_header.un.echo.sequence);
	printf("packets transmitted, %d ", ping_data->packets_received);
	printf("packets received, %.0f%% packet loss\n", loss);
	if (ping_data->packets_received == 0)
		return ;
	printf("round-trip min/avg/max/stddev = %.3f/", ping_data->min_time);
	printf("%.3f/", ping_data->sum_times / ping_data->packets_received);
	printf("%.3f/", ping_data->max_time);
	printf("%.3f", sqrt(ping_data->square_dist / ping_data->packets_received));
	printf(" ms\n");
}
