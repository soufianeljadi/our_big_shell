/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 23:52:10 by sdiouane          #+#    #+#             */
/*   Updated: 2024/06/05 12:21:11 by sdiouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	supprimerguillemets__(char *chaine)
{
	int		i;
	int		j;

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

void	supprimerguillemetsdoll(char *chaine)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (chaine[i])
	{
		if (chaine[i] != '$')
			chaine[j++] = chaine[i];
		i++;
	}
	chaine[j] = '\0';
}

int	is_single(char *str)
{
	int		s;
	int		d;

	s = 0;
	d = 0;
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '\'' && !d)
			s = !s;
		if (str[i] == '"' && !s)
			d = !d;
		if ((str[i] == '$' && (str[i + 1] == ' ' || str[i + 1] == '\0' || str[i + 1] == '\t')) ||
			(str[i] == '$' && (str[i + 1] == '"' || str[i + 1] == '\'')))
			return 1;
		if (s || d)
			return (1);
	}
	return (0);
}

ExecutionData	*init_data(char **args, noued_cmd *cmd, s_env *export_i)
{
	ExecutionData	*data;

	data = (ExecutionData *)malloc(sizeof(ExecutionData));
	if (!data)
		return (NULL);
	data->lst = cmd;
	data->args = args;
	data->export_i = export_i;
	return (data);
}

int	check_delem(char *delem)
{
	if (strcmp(delem, "<") || strcmp(delem, "<<") || strcmp(delem, ">")
		|| strcmp(delem, ">>") || strcmp(delem, "|"))
		return (0);
	else
		return (1);
}

int file_exists(const char* filename)
{
    struct dirent *entry;
	
    DIR *dir = opendir(".");
    if (dir == NULL)
        return 0;
    while ((entry = readdir(dir))!= NULL)
	{
        if (strcmp(entry->d_name, filename) == 0)
		{
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

char* generate_name(char* base_name)
{
    char* filename = malloc(strlen(base_name));

    if (file_exists(filename))
	{
        free(filename);
        return generate_name(base_name);
    }
    return filename;
}

char *expand_in_heredoc(char *commande, s_env *export_i)
{
	char	*exp_commande;
	t_p		p = {0, 0, 0, '\0'};

	if (!commande)
		exit(exit_stat(1));
	exp_commande = ft_strdup(commande);
	check_memory_allocation(exp_commande);
	while (exp_commande && exp_commande[p.i] != '\0')
	{
		handle_quotes(exp_commande, &p);
		if (exp_commande[p.i] == '$' && exp_commande[p.i + 1] == '?')
			exp_commande = ft_str_replace(exp_commande, ft_strdup("$?"), ft_itoa(exit_stat(-1)));
		if (exp_commande[p.i] == '$')
		{
			exp_commande = process_variable(exp_commande, &p, export_i);
			check_memory_allocation(exp_commande);
		}
		p.i++;
	}
	return (exp_commande);
}

void	handle_heredocs(char **delem, ExecutionData *data)
{
	int		fd;
	char	*buf;
	char	*file_name;
	int		flag;
	int		p;
	
	p = 0;
	flag = 0;
	file_name = generate_name("tmp.txt");
	fd = open("tmp.txt", O_TRUNC | O_CREAT | O_RDWR, 0777);
	buf = readline("heredocs >> ");
	if (strstr(*delem, "'") || strstr(*delem, "\""))
	{
		flag = 1;
		if (*delem[0] == '"' || *delem[0] == '\'')
			*delem = ft_substr2(*delem, 1, ft_strlen(*delem) - 2);
		else
			supprimerGuillemets(*delem);
	}
	while (1)
	{
		if (strcmp(buf, *delem) == 0)
			break ;
		if (flag == 0)
			buf = expand_in_heredoc(buf, data->export_i);
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		buf = readline("heredocs >> ");
	}
	free(buf);
	close(fd);
	*delem = ft_strdup("tmp.txt");
}

void	check_here_doc(ExecutionData *data)
{
	int	i;

	i = 0;
	if (!data->args)
		return ;
	while (data->args[i])
	{
		if (!strcmp(data->args[i], "<<") && data->args[i + 1])
			handle_heredocs(&data->args[i + 1], data);
		i++;
	}
}

void	loop_fct(ExecutionData *data, char *line)
{
	char	*pwd;
	
	pwd = NULL;
	while (42)
	{
		(pwd = print_directory(pwd), line = readline(pwd));
		if (!line)
			(printf("exit\n"),exit_stat(0));
		if(line != NULL && only_spaces(line) == 0)
		{
			add_history(line);
			if(parsing(line, data) == 1)
			{
				syntax_error();
				exit_stat(258);
			}
			else
			{
				data->args = line_to_args(line);
				check_here_doc(data);
				data->lst = split_args_by_pipe(data->args);
				data->lst = ft_expanding(&data, data->export_i);
				// print_command_list(data->lst);
				(dup2(0, 3),dup2(1, 4), ft_execution(data));
				(dup2(3, 0), dup2(4, 1), close(3), close(4));
				(free (data->args)/*, free_noued_cmd(data->lst)*/);
			}
		}
		// else
			// exit_stat(66048);    
	}
}

void	main_loop(char *line, s_env *export_i)
{
	char			**args;
	noued_cmd		*cmd;
	ExecutionData	*data;

	data = NULL;
	args = NULL;
	cmd = NULL;
	data = init_data(args, cmd, export_i);
	loop_fct(data, line);
	free(line);
	clear_history();
	free_noued_cmd(cmd);
	ft_free_tab(args);
}

void	f(void)
{
	system("leaks minishell");
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	s_env	*export_i;

	(void)av;
	export_i = NULL;
	line = NULL;
	if (ac != 1)
		(printf("Args not allowed !\n"), exit(EXIT_FAILURE));
	if (!isatty(STDIN_FILENO))
	{
		printf("minishell: is not a tty\n");
		exit(EXIT_FAILURE);
	}
	rl_catch_signals = 0;
	if (env[0] == NULL)
		export_i = split_export_i(export_i);
	else
		export_i = split_env(env);
	signals_init();
	main_loop(line, export_i);
	free_s_env(export_i);
	return (0);
}
