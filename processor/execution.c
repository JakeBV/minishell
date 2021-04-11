#include "minishell.h"

static int func_in_return(t_data *data, char **args, void (*f)(t_data *, char **))
{
	f(data, args);
	return (1);
}

static int exec_my_function(char **args, t_data *data)
{
	if (!ft_strcmp(args[0], "exit"))
		return (shell_exit(args, 0, data));
	else if (!ft_strcmp(args[0], "cd"))
		return (func_in_return(data, args, shell_cd));
	else if (!ft_strcmp(args[0], "export"))
		return (func_in_return(data, args, shell_export));
	else if (!ft_strcmp(args[0], "unset"))
		return (func_in_return(data, args, shell_unset));
	args[0] = ft_str_to_lower(args[0]);
	if (!ft_strcmp(args[0], "echo"))
		return (func_in_return(data, args, shell_echo));
	else if (!ft_strcmp(args[0], "pwd"))
		return (func_in_return(data, args, shell_pwd));
	else if (!ft_strcmp(args[0], "env"))
		return (func_in_return(data, args, shell_env));
	return (0);
}

static void parent_process(t_data *data)
{
	dup2(data->orig_fd[0], 0);
	dup2(data->orig_fd[1], 1);
	waitpid(-1, &g_status, 0);

	if (WIFEXITED(g_status))
	{
		//printf("exit status %d\n", status);
		g_status = WEXITSTATUS(g_status);
		printf("exit code = %d\n", g_status);
	}
	else if (WIFSIGNALED(g_status))
	{
		g_status = g_status | 128;
		printf("\nexit code = %d\n", g_status);
		if (g_status == 131)
			ft_putstr_fd("Quit: 3\n", 2);
	}
}

static void child_process(t_data *data, t_args *ar)
{
	int ret;

	ret = exec_my_function(ar->args, data);
	if (ret)
		exit(0);
	if (!ft_strchr(ar->args[0], '/'))
		find_function_path(ar, data->envlist);

	envlist_to_array(data);

	//debugging args
	int z = 0;
	printf("execve->[%s]\n", ar->args[0]);
	while(ar->args[++z])
		printf("args[%d]->[%s]\n", z, ar->args[z]);
	//end debug

	ret = execve(ar->args[0], ar->args, data->envp);
	if (errno == 2)
	{
		display_error("minishell", "command not found", ar->args[0]);
		ft_exit(127, data); // but what errno returns?
	}
	else
	{
		display_error("minishell", ar->args[0], "permission denied");
		ft_exit(126, data);
	}
}

int  execution(t_data *data)
{
	int ret;
	t_args *tmp;
	pid_t pid;

	tmp = data->ar;
	while (tmp)
	{
		if (tmp->args && tmp->args[0])
		{
			if (tmp->type == 0)
			{
				ret = exec_my_function(tmp->args, data);
				if (ret)
					return (0);
			}
			data->fd[0] = find_fdin(data);
			data->fd[1] = find_fdout(data, tmp->type);

			/*
					printf("data->orig_fd[0]:%d | data->orig_fd[1]:%d\n", data->orig_fd[0], data->orig_fd[1]);
					printf("data->fd[0]:%d | data->fd[1]:%d\n", data->fd[0], data->fd[1]);
					printf("data->pipe_fd[0]:%d | data->pipe_fd[1]:%d\n", data->pipe_fd[0], data->pipe_fd[1]);
			*/

			dup2(data->fd[0], 0);
			close(data->fd[0]);
			dup2(data->fd[1], 1);
			close(data->fd[1]);
			pid = fork();
			if (pid == 0) //child
			{
				child_process(data, tmp);
			} else //parent
			{
				g_lastpid = pid;
				parent_process(data);
			}
			reset_fd(data);
		}
		tmp = tmp->next;
	}
	return (0);
}
