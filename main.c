#include "minishell.h"

int g_status = 0;
int g_lastpid = 0;


void close_2_fd(int *fd)
{
	if (fd[0] >= 0)
		close(fd[0]);
	if (fd[1] >= 0)
		close(fd[1]);
}

void	close_all_redir_fd(t_data *data)
{
	t_args *tmp;

	tmp = data->ar;
	while (tmp)
	{
		close_2_fd(tmp->file);
		/*
		if (tmp->file[0] >= 0)
			close(tmp->file[0]);
		if (tmp->file[1] >= 0)
			close(tmp->file[1]);
		 */
		tmp = tmp->next;
	}
}

void reset_fd(t_data *data, int *file_fd)
{
	dup2(data->orig_fd[0], 0);
	dup2(data->orig_fd[1], 1);
	close_2_fd(file_fd);
}

/*
** There is a need for close_all_redir_fd(), in case parser returns -1
** -> it means something might be fucked up while opening files.
** But maybe it is unnecessary - think about it
*/

static void renew_data(t_data *data)
{
	close_all_redir_fd(data);
	args_clearlist(&data->ar);
}

static void init_shell(t_data *data, int argc, char **argv, char **env)
{
	data->prompt = "superbash> ";
	data->hist.len = 0;
	data->hist.list = NULL;
	data->hist.maxlen = HISTORY_MAX_LEN;
	if (argc != 1)
	{
		display_error("minishell", argv[1], "cannot execute this file");
		ft_exit(127, data);
	}
	data->envlist = get_envlist(env);
	data->orig_fd[0] = dup(0);
	data->orig_fd[1] = dup(1);
	data->pipe_fd[0] = 0;
	data->pipe_fd[1] = 0;
	data->envp = NULL;
}

void minishell(t_data *data)
{
	char *line;
	int count;
	int ret;

	line = "echo lalal > file5";
	ret = 0;
	//while ((line = ft_readline(data)) != NULL)
	//{
		if (line[0] != '\0')
		{
			count = 0;
			while (*(line + count))
			{
				ret = test_parser(line + count, count, data); // TODO: строку сначала давай в парсер виталика, а не line+count

				//debug parser
				printf("argslist size %d\n", argslstsize(data->ar));
				//end debug

				add_history(line, &data->hist); // Add to the list.
				save_history("list.txt"); // Save the list on disk.

				if (ret == -1)
				{
					renew_data(data); // TODO: нужно ли?
					g_status = 258; // TODO: может виталик присвоить его?
					break;
				}
				else
					execution(data);
				count += ret;
				renew_data(data);
			}
		}
	//	free(line);
	//}
}

int			main(int argc, char **argv, char **env)
{
	t_data	data;

	init_shell(&data, argc, argv, env);
	set_signals(&data);
	minishell(&data);

	return (0);
}
