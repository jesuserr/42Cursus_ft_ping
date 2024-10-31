/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:06:52 by jesuserr          #+#    #+#             */
/*   Updated: 2024/10/31 19:54:42 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "srcs/ft_ping.h"

int	main(int argc, char **argv)
{
	t_arguments	args;

	ft_bzero(&args, sizeof(t_arguments));
	parse_arguments(argc, argv, &args);
	printf("Destination: %s\n", args.destination);
	printf("Verbose mode: %d\n", args.verbose_mode);
	printf("Print timestamps: %d\n", args.print_timestamps);
	return (0);
}
