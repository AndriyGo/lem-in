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

t_list	*new_room(char *name)
{
	t_list	*ret;

	ret = ft_lstnew(name, (ft_strlen(name) + 1) * sizeof(char *));
	return (ret);
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

void	add_room(t_lemin *lemin, char *name)
{
	t_list	*room;

	room = new_room(name);
	if (lemin->rooms == NULL)
		lemin->rooms = room;
	else
		ft_lstappend(lemin->rooms, room);
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
