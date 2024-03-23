/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 14:48:45 by sdiouane          #+#    #+#             */
/*   Updated: 2024/03/23 15:48:15 by sdiouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pwd_without_options(char *line)
{
	int i = 0;
	char *pwd;
	
	pwd = getcwd(NULL, 0);

	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (strncmp(&line[i], "pwd", 3) == 0)
	{
		if (pwd != NULL)
		{
			printf("%s\n", pwd);
			free(pwd);
		}
		else
			perror("getcwd() error");
	}
}