#include "minishell.h"

void	ft_exit(int errcode, t_data *data, char hist)
{
	close_all_redir_fd(data);
	close(data->orig_fd[0]);
	close(data->orig_fd[1]);
	if (errcode == -1 && errno)
		display_error("minishell", NULL, strerror(errno));
	if (hist == 1)
		save_history(data);
	exit(errcode);
}

int	shell_exit(char **args, int errcode, t_data *data)
{
	if (args[0] && args[1])
	{
		if (!ft_str_is_numeric(args[1]))
		{
			display_error("minishell : exit", args[1],
				 "numeric argument required");
			ft_exit(-1, data, 1);
		}
		else if (args[2])
		{
			display_error("minishell", "exit", "too many arguments");
			g_struct.status = 1;
			return (1);
		}
		else
			errcode = ft_atoi(args[1]);
	}
	ft_exit(errcode, data, 1);
	return (1);
}

void	display_error(char *commandname, char *errname, char *str)
{
	if (commandname)
	{
		ft_putstr_fd(commandname, 2);
		ft_putstr_fd(": ", 2);
	}
	if (errname)
	{
		ft_putstr_fd(errname, 2);
		ft_putstr_fd(": ", 2);
	}
	if (str)
		ft_putendl_fd(str, 2);
}

void	free_2d_array(char **array, int start)
{
	int	i;

	i = start;
	while (array[i] != NULL)
	{
		ft_free((void **)&array[i]);
		i++;
	}
	if (!start)
	{
		free(array);
		array = NULL;
	}
}
