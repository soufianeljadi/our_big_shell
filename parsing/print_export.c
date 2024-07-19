/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 22:02:27 by sel-jadi          #+#    #+#             */
/*   Updated: 2024/07/19 22:14:42 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_export(t_env *list)
{
	char	*tmp;

	tmp = NULL;
	while (list)
	{
		if (ft_strcmp(list->key, "_") != 0 && ft_strcmp(list->key, "?") != 0)
		{
			if (list->value == NULL)
				printf("declare -x %s\n", list->key);
			else
			{
				if (!ft_strcmp(list->value, ""))
				{
					printf("declare -x %s=", list->key);
					printf("\"%s\"\n", list->value);
				}
				else if (ft_strcmp(list->value, ""))
				{
					printf("declare -x %s=", list->key);
					tmp = ft_strdup(list->value);
					ft_rm_quotes(tmp);
					printf("\"%s\"\n", tmp);
					free (tmp);
				}
			}
		}
		list = list->next;
	}
}
