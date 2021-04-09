
#include "parseader.h"

bool behind_has_backslash(char *line, char current, char *set, int i)
{
	if (ft_strrchr(set, current) && ((i > 0 && line[i - 1] != '\\') || !i))
		return (False);
	return (True);
}

bool im_in_quotes(int i, int count, int *locations)
{
	int j;

	j = 0;
	while (j < count)
	{
		if (i < locations[j])
			break ;
		j++;
	}
	return (j % 2);
}

bool im_in_end(char *line, char needed, int len)
{
	if (line[len - 1] == needed && len > 2 && line[len - 2] != '\\')
		return (True);
	return (False);
}

bool im_in_begin(char *line, char needed)
{
	if (line[0] == needed)
		return (True);
	return (False);
}

bool im_alone_redirect(char *line, int len)
{
	if ((line[len - 1] ==  '>' || line[len - 1] == '<') && \
			(len == 1 || (len > 2 && line[len - 2] != '\\')))
		return (True);
	return (False);
}