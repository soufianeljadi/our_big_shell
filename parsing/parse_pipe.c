/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 23:08:35 by sdiouane          #+#    #+#             */
/*   Updated: 2024/06/08 10:22:43 by sdiouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_next(char *line, int i)
{
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] == '\0')
		return (1);
	while (line[i] != '|' && line[i] != '\0')
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\0')
			return (0);
		i++;
	}
	return (1);
}

int	parse_single_input(char *line, char c)
{
	int	i;
	int	r;

	i = 0;
	r = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '"')
			break ;
		if (line[i] == c)
		{
			r = check_next(line, i + 1);
			if (r == 1)
				return (1);
		}
		i++;
	}
	return (0);
}
