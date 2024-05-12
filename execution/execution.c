/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:14:48 by sdiouane          #+#    #+#             */
/*   Updated: 2024/05/12 19:31:16 by sdiouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void supprimerGuillemets(char *chaine)
// {
//     int i = 0;
// 	int j = 0;

//     while (chaine[i])
// 	{
//         if (chaine[i] != '"')
//             chaine[j++] = chaine[i];
//         i++;
//     }
//     chaine[j] = '\0';
// }

void del_qotes(char *chaine)
{
    int i = 0;
	int j = 0;
	
    while (chaine[i])
	{
        if (chaine[i] != '"')
            chaine[j++] = chaine[i];
        i++;
    }
    chaine[j] = '\0';
}

void sup(char *chaine)
{
    int i = 0;
	int j = 0;

    while (chaine[i])
	{
        if (chaine[i] != '\'')
            chaine[j++] = chaine[i];
        i++;
    }
    chaine[j] = '\0';
}

void supprimerGuillemets(char *chaine) {
    int i = 0, j = 0;

    while (chaine[i]) {
        if (chaine[i] != '\"' && chaine[i] != '\'')
            chaine[j++] = chaine[i];
        i++;
    }
    chaine[j] = '\0';
}


void execute(char *s, char **env)
{
	char	*chemin;
	char	**cmd = NULL;
	int i = 0; 

	if (*env)
	{
		if ((s[0] == '\"' || s[0] == '\'') && (strstr(s, " ") || strstr(s, "\t")))
			cmd = ft_split(s, ' ');
		else
		{
			if (!strcmp(s , ""))
			{
				return ;
			}
			else
				cmd = split_space_tab(s, ' ');
			if (cmd[0][0] == '\0')
				exit(EXIT_FAILURE);
			while (cmd[i])
				supprimerGuillemets(cmd[i++]);
		}
		chemin = get_path(cmd[0], env);
		if (execve(chemin, cmd, g_flags.envire) == -1 && !strstr(cmd[0], "$"))
		{
			fprintf(stderr, "minishell : %s %s\n", cmd[0], strerror(errno));
			(ft_free_tab(cmd), exit(EXIT_FAILURE));
		}
	}
}

void add_last_cmd(s_env **lst, char **args)
{
	int i;
	s_env *tmp;
	
	i = 0;
	tmp = *lst;
	while (args[i])
		i++;
	while (tmp)
	{
		if (!strcmp(tmp->key, "_"))
			tmp->value = strdup(args[i - 1]);
		tmp = tmp->next;
	}
}

static int	ft_lstsize(s_env *lst)
{
	int		c;
	s_env	*p;

	c = 0;
	p = lst;
	while (p)
	{
		p = p -> next;
		c++;
	}
	return (c);
}

static char	**ft_merge_envr(s_env *export_i)
{
	char	**str;
	int		len;
	int		i;

	i = 0;
	len = ft_lstsize(export_i);
	str = NULL;
	str = malloc(sizeof(char *) * (len + 1));
	if (!str)
		return (NULL);
	while (export_i)
	{
		str[i] = ft_strdup("");
		str[i] = ft_strjoin(str[i], export_i->key);
		if (export_i->value)
		{
			str[i] = ft_strjoin(str[i], "=");
			str[i] = ft_strjoin(str[i], export_i->value);
		}
		export_i = export_i->next;
		i++;
	}
	str[i] = NULL;
	return (str);
}

static void handle_child_process(ExecutionData *data)
{;
    if (data->lst->redirection != NULL)
	{
		if (builtins(data) == 1)
        	execute_with_redirection(data);
		exit(EXIT_SUCCESS);
    }             
	else
	{
		if (data->lst->cmd != NULL)
		{
			if (builtins(data) == 1)
				execute(data->lst->cmd, data->env);
			exit(EXIT_SUCCESS);
		}
    }
} 

static void execute_command(ExecutionData *data)
{
	int pipefd[2];
    pid_t pid;

	if (pipe(pipefd) == -1 || (pid = fork()) == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
	{
		    if (data->lst->next != NULL)
        		dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
			handle_child_process(data);
	}
	else
	{
		    dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
	}
}

char **struct_to_char(s_env *lst)
{
	char **env;
	int i;
	s_env *tmp;

	i = 0;
	tmp = lst;
	env = malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
	while (tmp)
	{
		env[i] = ft_strjoin(tmp->key, "=");
		if (tmp->value)
			env[i] = ft_strjoin(env[i], tmp->value);
		tmp = tmp->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

void	ft_execution(ExecutionData *data)
{
	char	**env;

	env = NULL;
	env = struct_to_char(data->export_i);
	data->env = env;
    add_last_cmd(&data->export_i, data->args);
	if (data->lst->next == NULL && strcmp(data->lst->cmd, ""))
	{
		if (builtins(data) == 1)
			execute_command(data);
	}
	else
	{       
		while (data->lst)
		{
			g_flags.envire = ft_merge_envr(data->export_i);
			execute_command(data);
			data->lst = data->lst->next;
		}
	}
    while (0 < wait(NULL))
		;
}
