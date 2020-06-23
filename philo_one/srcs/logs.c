/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmartin <lmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 00:03:02 by lmartin           #+#    #+#             */
/*   Updated: 2020/06/23 22:09:29 by lmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** function: {msg_error}
**
** parameters:
** (char *){prg_name} - program's name,
** (char *){msg} - error's message (ex: malloc error)
**
** return (void)
**
** description:
** send an error in stderr_fileno as:
** "prg_name: msg_error"
*/

void	msg_error(char *prg_name, char *msg)
{
	write(STDERR_FILENO, prg_name, ft_strlen(prg_name));
	write(STDERR_FILENO, msg, ft_strlen(msg));
}

/*
** function: {throw_error}
**
** parameters:
** (char *){prg_name} - program's name,
** (int) {error} - error's number
**
** return (int): error's number
**
** description:
** call msg_error by corresponding the given error code {error} with a message
*/

int		throw_error(char *prg_name, int error)
{
	if (error == TOO_MANY_ARGS)
		msg_error(prg_name, ": Wrong number of arguments\n");
	else if (error == WRONG_ARG)
		msg_error(prg_name, ": Wrong argument\n");
	else if (error == ERROR_MALLOC)
		msg_error(prg_name, ": malloc error\n");
	else if (error == ERROR_MUTEX)
		msg_error(prg_name, ": mutex error\n");
	else if (error == ERROR_PTHREAD)
		msg_error(prg_name, ": pthread error\n");
	else if (error == ERROR_SLEEP)
		msg_error(prg_name, ": usleep error\n");
	else if (error == ERROR_TIMEOFDAY)
		msg_error(prg_name, ": gettimeofday error\n");
	return (error);
}

/*
** function: {logs}
**
** parameters:
** (timeval *){st} - timeval start of the program,
** (timeval *){tv} - actual timeval,
** (size_t){number} - philosopher's number,
** (char *){msg} - msg of the current philosopher
**
** return (int): no-null if error
**
** description:
** print on stdin the status of the {number} philosopher at a time {tv} like :
** "timestamp_in_ms number msg"
*/

int		logs(struct timeval *st, struct timeval *tv, size_t number, char *msg)
{
	char		*ptr;
	char		*log;
	size_t		size_nb;
	long long	temp;
	long long	time;

	temp = number * 10;
	size_nb = 4 + ft_strlen(msg);
	while (temp /= 10)
		size_nb++;
	time = (tv->tv_sec - st->tv_sec) * 1000 +
(tv->tv_usec - st->tv_usec) * 0.001;
	temp = time;
	while (temp /= 10)
		size_nb++;
	if (!(log = malloc(sizeof(char) * (size_nb))))
		return (ERROR_MALLOC);
	ptr = log;
	fill_nbr((size_t)time, &ptr);
	fill_msg("ms ", &ptr);
	fill_nbr(number, &ptr);
	fill_msg(msg, &ptr);
	write(STDOUT_FILENO, log, size_nb);
	free(log);
	return (0);
}
