/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 23:00:01 by sdiouane          #+#    #+#             */
/*   Updated: 2024/06/05 10:12:52 by sdiouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *file_nc(char *s)
{
	int i;
	int x;
	char *f;
	
	i = 0;
	x = 0;
	if (!s)
		return (NULL);
	f = (char *)malloc(sizeof(char) * (strlen(s) + 1));
	if (f == NULL)
		exit(EXIT_FAILURE);
	while (s[i] != '\0')
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			char quote = s[i];
			i++;
			while (s[i] != '\0' && s[i] != quote)
			{
				f[x] = s[i];
				i++;
				x++;
			}
			if (s[i] == '\0')
				exit(EXIT_FAILURE);
		}
		else
		{
			f[x] = s[i];
			x++;
		}
		i++;
	}
	f[x] = '\0';
	return f;
}

void redirection_double_out(char *redirection, int *fd)
{
	if (redirection)
	{
        *fd = open(file_nc(redirection), O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (*fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(redirection, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			exit(EXIT_FAILURE);
		}
		redirection = NULL;
        dup2(*fd, STDOUT_FILENO);
        close(*fd);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redirection, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		exit(EXIT_FAILURE);
	}
}

void redirection_in(char *redirection, int *fd)
{
	if (redirection != NULL)
	{
		*fd = open(file_nc(redirection), O_RDONLY);
		if (*fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(redirection, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			exit(EXIT_FAILURE);
		}
		printf("-------> file : %s\n", redirection);
		dup2(*fd, STDIN_FILENO);
		close(*fd);
		// unlink(redirection);
		redirection = NULL;
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redirection, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		exit(EXIT_FAILURE);
	}
}

void redirection_out(char *redirection, int *fd)
{
	// del_qotes1(redirection);
	if (redirection)
	{
        *fd = open(file_nc(redirection), O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (*fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(redirection, 2);
			ft_putstr_fd(" : ", 2);
			ft_putendl_fd(strerror(errno), 2);
            exit(EXIT_FAILURE);
        }
		redirection = NULL;
        dup2(*fd, STDOUT_FILENO);
        close(*fd);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redirection, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		exit(EXIT_FAILURE);
	}
	// printf("here222\n");
}

void execute_with_redirection(ExecutionData *data)
{
    int fd_in;
    int fd_out;
	int i;
	char **red;

	(1) && (fd_in = 0, fd_out = 1, i = 0,
	red = line_to_args(data->lst->redirection));
	if (!red)
		return ;
	while (red[i])
	{
		if (!strcmp(red[i], "<") || !strcmp(red[i], "<<"))
			redirection_in(red[i + 1], &fd_in);
		if (!strcmp(red[i], ">>"))
			redirection_double_out(red[i + 1], &fd_out);
		if (!strcmp(red[i], ">"))
			redirection_out(red[i + 1], &fd_out);
		i++;
	}
	if (data->lst->cmd != NULL && strspn(data->lst->cmd, " ") != strlen(data->lst->cmd))
		execute(data->lst->cmd, data->env, data);
}
