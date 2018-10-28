#include "lemin.h"
#include <stdio.h>

int	disregard_comment(t_lemin *lemin, char *line)
{
	if (line[0] != '#')
		return (0);
	if (ft_strcmp(line, "##start") == 0)
	{
		if ((lemin->rooms != NULL) || (lemin->mode != ROOMS) || \
			(lemin->op != NONE))
			return (0);
		else
			lemin->op = START;
	}
	else if (ft_strcmp(line, "##end") == 0)
	{
		if ((lemin->end_found == 1) || (lemin->rooms == NULL) || \
			(lemin->mode != ROOMS) || (lemin->op != NONE))
			return (0);
		else
			lemin->op = END;
	}
	return (1);
}

int	coordinate_valid(char *line, int len)
{
	int	c;

	if ((line == NULL) || (len == 0))
		return (0);
	c = 0;
	if (line[0] == '-')
	{
		c++;
		line++;
	}
	while (c++ < len)
		if (ft_isdigit(*(line++)) == 0)
			return (0);
	return (c > 0);
}

int	read_room(t_lemin *lemin, char *line)
{
	char	*s;
	int		len;

	if ((lemin->op != START) && (lemin->rooms == NULL))
		return (0);
	if ((line[0] == 'L') || ((s = ft_strrchr(line, ' ')) == NULL))
		return (0);
	if (!coordinate_valid(s + 1, ft_strlen(line) - 1 - (s - line)))
		return (0);
	len = s - line;
	line[len] = '!';
	if ((len <= 0) || ((s = ft_strrchr(line, ' ')) == NULL))
		return (0);
	if ((!coordinate_valid(s + 1, len - 1 - (s - line))) || (s - line == 0))
		return (0);
	len = s - line;
	s = ft_strsub(line, 0, len);
	if (room_with_name_exists(lemin, s))
	{
		free(s);
		return (0);
	}
	add_room(lemin, s);
	free(s);
	if (lemin->op == END)
	{
		lemin->mode = LINKS;
		lemin->end_found = 1;
	}
	lemin->op = NONE;
	return (1);
}

int	read_ants(t_lemin *lemin, char *line)
{
	char	*tmp;

	tmp = line;
	while(*tmp)
		if (!ft_isdigit(*(tmp++)))
			return (0);
	lemin->ant_count = ft_atoi(line);
	lemin->mode = ROOMS;
	return (1);
}

int	room_with_name_exists(t_lemin *lemin, char *name)
{
	t_list *list;

	if (lemin->rooms == NULL)
		return (0);
	list = lemin->rooms;
	while (list != NULL)
	{
		if (ft_strcmp(((char *)list->content), name) == 0)
			return (1);
		list = list->next;
	}
	return (0);
}

int	read_link(t_lemin *lemin, char *line)
{
	char	*r1;
	char	*r2;
	char	*tmp;

	tmp = line;
	while (*tmp)
	{
		r1 = ft_strsub(line, 0, tmp - line + 1);
		if (room_with_name_exists(lemin, r1) && (*(tmp + 1) == '-'))
		{
			r2 = ft_strsub(tmp + 2, 0, ft_strlen(tmp) - 2);
			if (room_with_name_exists(lemin, r2))
			{
				add_link(lemin, r1, r2);
				free(r1);
				free(r2);
				return (1);
			}
			free(r2);
		}
		free(r1);
		tmp++;
	}
	return (0);
}

int	main(void)
{
	t_lemin	*lemin;
	char	*line;
	int		stop;

	init_lemin(&lemin);
	stop = 0;
	while (get_next_line(1, &line) > 0)
	{
		if (lemin->op == START)
			stop = (read_room(lemin, line) == 0);
		else if (lemin->op == END)
			stop = (read_room(lemin, line) == 0);
		else if (line[0] == '#')
			stop = (disregard_comment(lemin, line) == 0);
		else if (lemin->mode == ANTS_N)
			stop = (read_ants(lemin, line) == 0);
		else if (lemin->mode == ROOMS)
			stop = (read_room(lemin, line) == 0);
		else if (lemin->mode == LINKS)
			stop = ((ft_strlen(line) < 3) || (read_link(lemin, line) == 0));
		free(line);
		if (stop)
			break;
	}
	die();
	return (0);
}