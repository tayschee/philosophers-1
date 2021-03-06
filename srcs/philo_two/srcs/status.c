/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmartin <lmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 03:18:32 by lmartin           #+#    #+#             */
/*   Updated: 2020/07/17 04:23:07 by lmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

/*
** function: {eating}
**
** parameters:
** (t_philosopher *){phi} - philosopher's structure
**
** return (int) - error's code
**
** description:
** action of eat
*/

int			eating(t_philosopher *phi)
{
	if (!phi->time_last_meal)
	{
		if (sem_post(phi->sem_last_meal))
			throw_error(ERROR_SEM);
		return (1);
	}
	phi->nb_eat++;
	if (sem_post(phi->sem_last_meal))
		throw_error(ERROR_SEM);
	if (gettimeofday(phi->time_last_meal, NULL))
		throw_error(ERROR_TIMEOFDAY);
	logs(phi->parameters->time_start, phi->time_last_meal,
phi->nb, " is eating\n");
	ft_usleep(phi->parameters->time_to_eat * 1000);
	return (0);
}

/*
** function: {check_eating}
**
** parameters:
** (t_philosopher *){phi} - philosopher's structure
**
** return (int): no-null number if the philosopher is dead
**
** description:
** action of eat of a philosopher, it look at the last meal, and replace it if
** the philosopher just ate in time
*/

int			check_eating(t_philosopher *phi)
{
	int				ret;
	struct timeval	time_action;

	if (sem_wait(phi->sem_last_meal))
		throw_error(ERROR_SEM);
	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	if (phi->time_last_meal && ((size_t)((time_action.tv_sec -
phi->time_last_meal->tv_sec) * 1000 + (time_action.tv_usec -
phi->time_last_meal->tv_usec) * 0.001) > phi->parameters->time_to_die))
	{
		free(phi->time_last_meal);
		logs(phi->parameters->time_start,
&time_action, phi->nb, " died\n");
		phi->time_last_meal = NULL;
	}
	ret = eating(phi);
	if (sem_post(phi->parameters->forks))
		throw_error(ERROR_SEM);
	if (sem_post(phi->parameters->forks))
		throw_error(ERROR_SEM);
	return (ret);
}

/*
** function: {taking_forks}
**
** parameters:
** (t_philosopher *){phi} - philosopher's structure
**
** return (int) - return no-null if a philosopher has died during process
**
** description:
** each philosophers must take 2 forks before eating.
** {phi->parameters->available_eat} is done to avoid that philosophers take all
** 1 fork and deadlock
*/

int			taking_forks(t_philosopher *phi)
{
	struct timeval	time_action;

	if (!phi->time_last_meal)
		return (1);
	if (sem_wait(phi->parameters->available_eat))
		throw_error(ERROR_SEM);
	if (sem_wait(phi->parameters->forks))
		throw_error(ERROR_SEM);
	if (sem_wait(phi->parameters->forks))
		throw_error(ERROR_SEM);
	if (sem_post(phi->parameters->available_eat))
		throw_error(ERROR_SEM);
	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	logs(phi->parameters->time_start, &time_action, phi->nb,
" has taken a fork\n");
	logs(phi->parameters->time_start, &time_action, phi->nb,
" has taken a fork\n");
	return (0);
}

/*
** function: {routine}
**
** parameters:
** (t_philosopher *){phi} - philosopher
**
** return (int) - no-null if death
**
** description:
** routine of the philosopher (thinking - eating - sleeping)
*/

int			routine(t_philosopher *phi)
{
	struct timeval	time_action;

	if (check_eating(phi) || (phi->nb_eat ==
phi->parameters->number_of_time_each_philosophers_must_eat))
		return (1);
	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	logs(phi->parameters->time_start,
&time_action, phi->nb, " is sleeping\n");
	ft_usleep(phi->parameters->time_to_sleep * 1000);
	if (!phi->time_last_meal)
		return (1);
	if (gettimeofday(&time_action, NULL))
		throw_error(ERROR_TIMEOFDAY);
	logs(phi->parameters->time_start,
&time_action, phi->nb, " is thinking\n");
	return (0);
}

/*
** function: {alive}
**
** parameters:
** (void *){args} - contain philosopher's structure
**
** return (void *) - just exiting if the philosopher is dead
**
** description:
** thinking - eating - spleeping on a loop with timer, it call {eating} when
** it's the action's turn, if {eating} returns a no-null value, it mean that the
** philosopher died so it exit the function resulting of a kill of the pthread.
*/

void		*alive(void *args)
{
	t_philosopher	*phi;

	phi = (t_philosopher *)args;
	while (!taking_forks(phi) && !routine(phi))
		NULL;
	return (NULL);
}
