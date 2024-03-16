/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lsts.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 00:32:13 by sdiouane          #+#    #+#             */
/*   Updated: 2024/03/16 17:22:16 by sdiouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


s_cmd	*ft_lstnew()
{
	s_cmd	*b;

	b = (s_cmd *)malloc(sizeof(s_cmd));
	if (!b)
		exit(EXIT_FAILURE);
	b ->cmd_sep = NULL;
	b ->next = NULL;
	return (b);
}

s_cmd	*ft_lstnew_data(char *data)
{
	s_cmd	*b;

	b = (s_cmd *)malloc(sizeof(s_cmd));
	if (!b)
		exit(EXIT_FAILURE);
	b ->cmd_sep = data;
	b ->next = NULL;
	return (b);
}

void	ft_lstadd_back(s_cmd **lst, s_cmd *new)
{
	s_cmd	*tmp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
    

}

void	print_list(s_cmd *list)
{
	int i = 1;
  while (list)
  {
	printf("%d\t\t", i);
  	printf("%s\n", list->cmd_sep);
  	list = list->next;
	i++;
  }
}