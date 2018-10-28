#include "lemin.h"

t_list	*new_link(char *r1, char *r2)
{
	t_list	*ret;
	char	**link;

	if ((link = ft_memalloc(2 * sizeof(char *))) == NULL)
		exit(-1);
	ret = ft_lstnew(link, 2 * sizeof(char *));
	((char **)(ret->content))[0] = ft_strdup(r1);
	((char **)(ret->content))[1] = ft_strdup(r2);
	free(link);
	return (ret);
}

void	new_room(t_room	**ret, char *name, int x, int y)
{
	if ((*ret = ft_memalloc(sizeof(t_room))) == NULL)
		exit(-1);
	(*ret)->name = ft_strdup(name);
	(*ret)->next = NULL;
	(*ret)->x = x;
	(*ret)->y = y;
}

void	init_lemin(t_lemin **lemin)
{
	if ((*lemin = ft_memalloc(sizeof(t_lemin))) == NULL)
		exit(-1);
	(*lemin)->ant_count = 0;
	(*lemin)->rooms = NULL;
	(*lemin)->links = NULL;
	(*lemin)->op = NONE;
	(*lemin)->end_found = 0;
	(*lemin)->mode = ANTS_N;
}

void	add_room(t_lemin *lemin, char *name, int x, int y)
{
	t_room	*room;
	t_room	*tmp;

	new_room(&room, name, x, y);
	room->status = lemin->mode;
	if (lemin->rooms == NULL)
		lemin->rooms = room;
	else if (lemin->mode == START)
	{
		tmp = lemin->rooms;
		room->next = tmp;
		lemin->rooms = room;
	}
	else
	{
		tmp = lemin->rooms;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = room;
	}
}

void	delete_last_link(t_lemin *lemin)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = lemin->links;
	prev = NULL;
	while (tmp->next != NULL)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	free(tmp->content);
	free(tmp);
	if (prev == NULL)
		lemin->links = NULL;
	else
		prev->next = NULL;
	return ;
}

void	add_link(t_lemin *lemin, char *r1, char *r2)
{
	t_list	*link;

	link = new_link(r1, r2);
	if (lemin->links == NULL)
		lemin->links = link;
	else
		ft_lstappend(lemin->links, link);
}
