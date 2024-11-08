/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:31:42 by jesuserr          #+#    #+#             */
/*   Updated: 2024/11/08 19:53:38 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// PING ivoice.synology.me (79.154.85.235): 56 data bytes
// PING ivoice.synology.me (79.154.85.235): 56 data bytes, id 0x0f90 = 3984
void	print_header(t_ping_data *ping_data)
{
	if (inet_ntop(AF_INET, &(ping_data->dest_addr.sin_addr), ping_data->ip_str, \
	INET_ADDRSTRLEN) == NULL)
		print_perror_and_exit("inet_ntop header", ping_data);
	printf("PING %s (%s): ", ping_data->args.dest, ping_data->ip_str);
	printf("%ld data bytes", sizeof(t_icmp_packet) - sizeof(struct icmphdr));
	if (ping_data->args.verbose_mode)
	{
		printf(", id 0x%04x =", ping_data->packet.icmp_header.un.echo.id);
		printf(" %d", ping_data->packet.icmp_header.un.echo.id);
	}
	printf("\n");
}

// 64 bytes from 79.154.85.235: icmp_seq=0 ttl=165 time=0.914 ms
// Uses Welford's algorithm to calculate the population standard deviation on
// the fly (just in one single pass).
void	print_response_line(t_ping_data *ping_data, t_icmp_packet packet, \
		uint8_t ttl)
{
	struct timeval	tv;	
	float			time_ms;
	float			delta_time;

	ping_data->packets_received++;
	if (gettimeofday(&tv, NULL) == -1)
		print_perror_and_exit("gettimeofday receive packet", ping_data);
	if (ping_data->args.print_timestamps && !ping_data->args.quiet_mode)
		printf("[%ld.%ld] ", tv.tv_sec, tv.tv_usec);
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
	if (ping_data->args.quiet_mode)
		return ;
	printf("%ld bytes from %s: ", sizeof(t_icmp_packet), ping_data->ip_str);
	printf("icmp_seq=%d ", packet.icmp_header.un.echo.sequence);
	printf("ttl=%d time=%.3f ms \n", ttl, time_ms);
}

// buff contains the time exceeded received packet, which consists of the source
// IP address (20 bytes) + ICMP header (8 bytes) + original IP header (20 bytes)
// + original echo request packet (64 bytes). Values between () are not taken by
// granted, provided just for reference. Access to original ICMP header is
// needed to get the sequence number of the original packet.
// If the packet is not addressed to us, it is discarded.
// From xxx.xxx.xxx.xxx icmp_seq=x Time to live exceeded
void	print_ttl_exceeded_line(t_ping_data *ping_data, char *buff, \
		struct iphdr *ip_header)
{
	struct icmphdr	*inner_icmp_header;
	char			src_addr_str[INET_ADDRSTRLEN];

	inner_icmp_header = (struct icmphdr *)(buff + (ip_header->ihl * 4) + \
	sizeof(struct icmphdr) + sizeof(struct iphdr));
	if (inner_icmp_header->un.echo.id != \
	ping_data->packet.icmp_header.un.echo.id)
		return ;
	if (inet_ntop(AF_INET, &(ip_header->saddr), src_addr_str, INET_ADDRSTRLEN) \
	== NULL)
		print_perror_and_exit("inet_ntop ttl line", ping_data);
	printf("From %s icmp_seq=", src_addr_str);
	printf("%d Time to live exceeded\n", inner_icmp_header->un.echo.sequence);
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
