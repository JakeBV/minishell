#include "minishell.h"
#include "parseader.h"


void	pipes_processors(t_data *data, char *str, int i)
{
	int		ind;
	char	*new_str;

	ind = str_has_pipe(&data->pars, 0, i);
	while (ind)
	{
		new_str = ft_substr(str, i, ind - i); // TODO free old str!
		
		ind = str_has_pipe(&data->pars, ind, i);
	}
}

int		str_has_pipe(t_par *pars, int ind, int start)
{
	while (ind < pars->ppc)
	{
		if (pars->ppl[ind] < pars->next && pars->ppl[ind] > start)
			return (ind);
		ind++;
	}
	return (0);
}

char **space_splitter(t_par *pars, char *new_str, int i)
{
	int j;

	j = 0;
	if (i_inside_array(pars->sl, pars->sc, i, pars->next))
	{
		printf("ghjh\n");
	}
	else
		printf("HEY");
	return NULL;
}