#include "minishell.h"

void	delete_node(t_list_env **envs, char *name)
{
	t_list_env	*tmp;
	t_list_env	*del;

	tmp = *envs;
	while (tmp)
	{
		if (!ft_strcmp(tmp->next->name, name))
		{
			del = tmp->next;
			tmp->next = tmp->next->next;
			free(del->content);
			free(del->name);
			free(del);
			return ;
		}
		tmp = tmp->next;
	}
}

void	delete_var_from_list(t_list_env **envs, char *name)
{
	int	found;

	found = check_symbols(name);
	if (found)
	{
		g_struct.status = 1;
		display_error("minishell", "export", "not a valid identifier");
	}
	else
	{
		found = find_envvar(envs, name);
		if (found)
		{
			delete_node(envs, name);
		}
	}
}

void	shell_unset(t_data *data, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		delete_var_from_list(&data->envlist, args[i]);
		i++;
	}
}
