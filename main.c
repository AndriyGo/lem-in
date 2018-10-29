#include "lemin.h"
#include <stdio.h>

int	ft_atoi_no(const char *str)
{
	char	*prcs;
	long	ret;
	int		is_neg;
	int		step;

	prcs = (char *)str;
	while (*prcs && ft_iswhitespace(*prcs))
		prcs++;
	is_neg = (*prcs == '-');
	if (is_neg || (*prcs == '+'))
		prcs++;
	ret = 0;
	while (*prcs && ft_isdigit(*prcs))
	{
		step = (is_neg) ? ('0' - *prcs) : (*prcs - '0');
		if (is_neg && (ret < (-2147483648 - 1 - step) / 10))
			return (0);
		else if (!is_neg && (ret > (2147483647 - step) / 10))
			return (-1);
		else
			ret = ret * 10 + step;
		prcs++;
	}
	return ((int)ret);
}

int	disregard_comment(t_lemin *lemin, char *line)
{
	if (line[0] != '#')
		return (0);
	if (ft_strcmp(line, "##start") == 0)
	{
		if ((lemin->start_found == 1) || (lemin->mode != ROOMS) || \
			(lemin->op != NONE))
			return (0);
		else
		{
			lemin->op = START;
		}
	}
	else if (ft_strcmp(line, "##end") == 0)
	{
		if ((lemin->end_found == 1) || \
			(lemin->mode != ROOMS) || (lemin->op != NONE))
			return (0);
		else
		{
			lemin->op = END;
		}
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
	int		y;
	int		x;

	if ((line[0] == 'L') || ((s = ft_strrchr(line, ' ')) == NULL))
		return (0);
	if (!coordinate_valid(s + 1, ft_strlen(line) - 1 - (s - line)))
		return (0);
	y = ft_atoi(s + 1);
	len = s - line;
	line[len] = '!';
	if ((len <= 0) || ((s = ft_strrchr(line, ' ')) == NULL))
		return (0);
	if ((!coordinate_valid(s + 1, len - 1 - (s - line))) || (s - line == 0))
		return (0);
	len = s - line;
	x = ft_atoi(s + 1);
	s = ft_strsub(line, 0, len);
	if (room_exists(lemin, s, x, y))
	{
		free(s);
		return (0);
	}
	add_room(lemin, s, x, y);
	free(s);
	if (lemin->op == END)
		lemin->end_found = 1;
	else if (lemin->op == START)
		lemin->start_found = 1;
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
	lemin->ant_count = ft_atoi_no(line);
	lemin->mode = ROOMS;
	return (lemin->ant_count > 0);
}

int	room_with_name_exists(t_lemin *lemin, char *name)
{
	t_room *list;

	if (lemin->rooms == NULL)
		return (0);
	list = lemin->rooms;
	while (list != NULL)
	{
		if (ft_strcmp(((char *)list->name), name) == 0)
			return (1);
		list = list->next;
	}
	return (0);
}

int	room_exists(t_lemin *lemin, char *name, int x, int y)
{
	t_room *list;

	if (lemin->rooms == NULL)
		return (0);
	list = lemin->rooms;
	while (list != NULL)
	{
		if (ft_strcmp(((char *)list->name), name) == 0)
			return (1);
		if ((list->x == x) && (list->y == y))
			return (1);
		list = list->next;
	}
	return (0);
}

int	link_exists(t_lemin *lemin, char *r1, char *r2)
{
	t_list	*tmp;

	if (ft_strcmp(r1, r2) == 0)
		return (1);
	tmp = lemin->links;
	while (tmp)
	{
		if ((ft_strcmp(r1, ((char **)(tmp->content))[0]) == 0) && \
			(ft_strcmp(r2, ((char **)(tmp->content))[1]) == 0))
			return (1);
		if ((ft_strcmp(r1, ((char **)(tmp->content))[1]) == 0) && \
			(ft_strcmp(r2, ((char **)(tmp->content))[0]) == 0))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}	

int	read_link(t_lemin *lemin, char *line)
{
	char	*r1;
	char	*r2;
	char	*tmp;
	char	set;

	tmp = line;
	set = 0;
	while (*tmp)
	{
		r1 = ft_strsub(line, 0, tmp - line + 1);
		if (room_with_name_exists(lemin, r1) && (*(tmp + 1) == '-'))
		{
			r2 = ft_strsub(tmp + 2, 0, ft_strlen(tmp) - 2);
			if (room_with_name_exists(lemin, r2))
			{
				if (link_exists(lemin, r1, r2))
				{
					free(r1);
					free(r2);
					return (1);
				}
				set++;
				if (set > 1)
				{
					free(r1);
					free(r2);
					delete_last_link(lemin);
					return (0);
				}
				add_link(lemin, r1, r2);
			}
			free(r2);
		}
		free(r1);
		tmp++;
	}
	return (set == 1);
}

void	stop_or_solve(t_lemin *lemin)
{
	t_room	*tmp;
	int		valid;

	if ((lemin->start_found == 0) || (lemin->end_found == 0) || \
		(lemin->rooms == NULL) || (lemin->links == NULL))
		die();
	find_solution(lemin);
	valid = 0;
	tmp = lemin->solution;
	while (tmp != NULL)
	{
		if (tmp->status == END)
		{
			valid = 1;
			break;
		}
		tmp = tmp->next;
	}
	if (valid)
		print_solution(lemin);
	else
		die();
}

int	main(void)
{
	t_lemin	*lemin;
	char	*line;
	int		stop;

	init_lemin(&lemin);
	stop = 0;
	while (get_next_line(0, &line) > 0)
	{
		if (line[0] == '#')
			stop = (disregard_comment(lemin, line) == 0);
		else if (lemin->op == START)
			stop = (read_room(lemin, line) == 0);
		else if (lemin->op == END)
			stop = (read_room(lemin, line) == 0);
		else if (lemin->mode == ANTS_N)
			stop = (read_ants(lemin, line) == 0);
		else if (lemin->mode == ROOMS)
		{
			if (read_room(lemin, line) == 0)
			{
				lemin->mode = LINKS;
				stop = ((ft_strlen(line) < 3) || (read_link(lemin, line) == 0));
			}
		}
		else if (lemin->mode == LINKS)
			stop = ((ft_strlen(line) < 3) || (read_link(lemin, line) == 0));
		free(line);
		if (stop)
			break;
	}
	stop_or_solve(lemin);
	sleep(3);
	return (0);
}