/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_not_null.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 21:25:42 by sdiouane          #+#    #+#             */
/*   Updated: 2024/06/04 22:10:19 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_q(char *chaine)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (chaine[i])
	{
		if (chaine[i] != '\'')
			chaine[j++] = chaine[i];
		i++;
	}
	chaine[j] = '\0';
}

s_env	*process_export_args(char **args, s_env *env)
{
	int		i;
	int		j;
	char	*key;

	(1) && (i = 1, key = NULL);
	while (args[i])
	{
		if (strstr(args[i], " ") || strstr(args[i], "\t"))
			del_dbl_quotes(args[i]);
		j = 0;
		while (args[i][j] && args[i][j] != '=' && args[i][j] != '+')
			j++;
		key = ft_substr(args[i], 0, j);
		if (args[i][j] == '+' && args[i][j + 1] != '=')
		{
			printf("export : %c, not a valid identifier", args[i][j]);
			exit_stat(1);
		}
		else if (verif_export(key) == 0)
		{
			if (args[i][j] == '=')
				(env->i = i, env->j = j), fct_equal(args, env, key);
			else if (args[i][j] == '+' && args[i][j + 1] == '=')
				(env->i = i, env->j = j), ftc_concatination(args, env, key);
			else if (existe_deja(key, env) == 0)
				ft_lstadd_back(&env, ft_lstnew_data(NULL, key));
		}
		else
		{
			fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", key);
			exit_stat(1);
		}
		i++;
	}
	return (env);
}

s_env	*not_null(char **args, s_env *env)
{
	if (!strcmp(args[0], "export"))
	{
		if (!args[1])
			print_export(env);
		else
			env = process_export_args(args, env);
	}
	return (env);
}
