/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 22:11:52 by sdiouane          #+#    #+#             */
/*   Updated: 2024/04/18 16:55:54 by sdiouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	verif_export(char *str)
{
	int	i;

	i = 0;
	if (str[i] >= '0' && str[i] <= '9')
		return (1);
	while (str[i])
	{
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')))
			return (1);
		i++;
	}
	return (0);
}

s_env *export_fct(char **args, s_env *env)
{
	int i = 1;
	int j;
	char *key;
	char *value;
	s_env *current;

	if (!strcmp(args[0], "export") && !args[1])
		print_export(env);
	else if (!strcmp(args[0], "export") && args[1])	
	{
		while (args[i])
		{
			j = 0;
			while (args[i][j] != '=' && args[i][j] != '+')
				j++;
			key = ft_substr(args[i], 0, j);
			printf("KEY : %s\n", key);
			if (verif_export(key) == 0)
			{
				if (args[i][j] == '\0')
					ft_lstadd_back(&env, ft_lstnew_data(strdup(""), key));
				else
				{
					if (args[i][j] == '=')
					{
						j++;
						int start;
						start = j;
						while (args[i][j] != ' ')
							j++;
						value = ft_substr(args[i], start, j - start);
						current = env;
						while (current != NULL && current->value)
						{
							if (strcmp(current->key, key) == 0)
							{
								free(current->value);
								current->value = value;
								break;
							}
							current = current->next;
						}
						if (current == NULL)
							ft_lstadd_back(&env, ft_lstnew_data(value, key));
					}
					else if (args[i][j] == '+' && args[i][j + 1] == '=')
					{
						j = j + 2;
						int start;
						start = j;
						while (args[i][j] != ' ')
							j++;
						value = ft_substr(args[i], start, j - start);
						current = env;
						while (current != NULL && current->value)
						{
							if (strcmp(current->key, key) == 0)
							{
								current->value = ft_strjoin(current->value, value);
								break ;
							}
							current = current->next;
							if (current == NULL)
								ft_lstadd_back(&env, ft_lstnew_data(value, key));						
						}
					}
				}
			}
			else
				syntax_error();
			i++;
		}	
	}
	return (env);
}
