/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 23:52:10 by sdiouane          #+#    #+#             */
/*   Updated: 2024/03/14 23:50:32 by sdiouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void init_env(char **env)
{
	s_env *lst;
	lst->env = env;
}
int only_spaces(char *str)
{
	int i = 0;
	while(str[i])
	{
		if(str[i] == ' ' || str[i] == '\t')
			i++;
		else
			return (0);	
	}
	return(1);
}

void nbr_quotes(char *str)
{
	int i = 0;
	int cmp1 = 0;
	int cmp2 = 0;

	while (str[i] != '\0')
	{
		if (str[i] == '"' && str[i - 1] != '\\')
			cmp1++;
		i++;
	}
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' && str[i - 1] != '\\')
			cmp2++;
		i++;
	}
	
	if (cmp1 % 2 != 0 || cmp2 % 2 != 0)
		printf("error\n");
}

int main(int ac, char **av, char **env)
{
	((void)ac, (void)av);

	
	init_env(env);
	// t_sep *cmd = NULL;
	// t_sep *tmp= NULL;
	// char **str;
	// int i = 0;
	
	char *line = NULL;
	while (1)
	{  
		/***********************************Read line*/
		if((line = readline("Minishell :$ "))!= NULL  && only_spaces(line) == 0)
		{
			if (!line)
			    break;
				rl_on_new_line();
			nbr_quotes(line);
			/***********************************History*/
			add_history(line);
			/***********************************Check for exit cmd*/
			if (strncmp(line, "exit", 4) == 0)
			{
				printf("exit\n");
			   free(line);
				break;
			 }
			/***********************************Execute the cmd*/
			// pid_t pid = fork();
			// if (pid == -1) {
			// 	perror("fork"); 
			// } else if (pid == 0) {  // Child process
			// 	execlp(line, line, NULL);
			// 	perror("exec");
			// 	exit(EXIT_FAILURE);
			// }
			// else // Parent process
			// 	waitpid(pid, NULL, 0);
			// if (strcmp(line, ";") && strcmp(line, "\"") && (strcmp(line, " ") || strcmp(line, "\t")) && strcmp(line, "\'"))
			// {
			// 	handle_char();
			// }
		}
		// str = ft_split(line, ';');
		// tmp = cmd;
		// while (str[i])
		// {
		//     ft_lstadd_back(&cmd, ft_lstnew(str[i]));
		//     i++;
		// }
		// cmd->next=NULL;
		// // printf("hgere\n");
		// while (tmp)
		// {
		//     printf("%s\n", tmp->cmd_sep);
		//     tmp = tmp->next;
		// }
		// print_list(tmp);
		// add_history(line);


		free(line);
	}
	return 0;
}

