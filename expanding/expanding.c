/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiouane <sdiouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 19:58:08 by sdiouane          #+#    #+#             */
/*   Updated: 2024/05/20 11:14:33 by sdiouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void check_memory_allocation(void *ptr)
{
    if (!ptr) exit(EXIT_FAILURE);
}

void handle_quotes(char *exp_commande, t_p *p) 
{
    if (exp_commande[p->i] == '"' || exp_commande[p->i] == '\'')
	{
        if (p->quote_open && exp_commande[p->i] == p->current_quote)
		{
            p->quote_open = 0;
            p->current_quote = '\0';
        }
		else if (!p->quote_open)
		{
            p->quote_open = 1;
            p->current_quote = exp_commande[p->i];
        }
    }
}

char *process_variable(char *exp_commande, t_p *p, s_env *export_i)
{
    char *key;
    char *value;
    char *full_key;
    char *exp_cmd;

    key = get_env_key(exp_commande, p->i);
    check_memory_allocation(key);
    value = get_env_value(key, export_i);
    if (!value || !strcmp(value, "") || !strcmp(value, " "))
	{
        exp_commande = ft_str_replace(exp_commande, key, strdup(""));
        check_memory_allocation(exp_commande);
        (free(key), free(value));
    }
	else
	{
        full_key = ft_strjoin("$", key);
        check_memory_allocation(full_key);
        exp_cmd = ft_str_replace(exp_commande, full_key, value);
        (check_memory_allocation(exp_cmd), free(exp_commande));
        exp_commande = strdup(exp_cmd);
        check_memory_allocation(exp_commande);
        (free(full_key), free(exp_cmd), free(key), free(value));
    }
    return exp_commande;
}

char *exp_fct(char *commande, s_env *export_i)
{
    char *exp_commande;
    t_p p = {0, 0, 0, '\0'};
    if (!commande) exit(EXIT_FAILURE);
    exp_commande = strdup(commande);
    check_memory_allocation(exp_commande);
    while (exp_commande && exp_commande[p.i] != '\0')
	{
        handle_quotes(exp_commande, &p);
        if (exp_commande[p.i] == '$' && (!p.quote_open || (p.quote_open && p.current_quote == '"')))
		{
            exp_commande = process_variable(exp_commande, &p, export_i);
            check_memory_allocation(exp_commande);
        }
        p.i++;
    }
	// if (strstr(exp_commande, "$") && is_closed(exp_commande, p.i) == 0)
        supprimerDoll(exp_commande);
    return exp_commande;
}

noued_cmd *ft_expanding(ExecutionData **data, s_env *export_i)
{
    ExecutionData *tmp = *data;
    noued_cmd *current = tmp->lst;

    while (current)
	{
        if (current->cmd)
            current->cmd = exp_fct(current->cmd, export_i);
        if (current->redirection)
            current->redirection = exp_fct(current->redirection, export_i);
        current = current->next;
    }
    current = tmp->lst;
    while (current)
        current = current->next;
    return (tmp->lst);
}
