#include "lemin.h"

void	print_list(t_room *list)
{
	while (list != NULL)
	{
		printf("%s->", (char *)list->name);
		list = list->next;
	}
	printf("\n");
}

int		solution_contains_room(t_lemin *lemin, char *name)
{
	t_room	*tmp;

	tmp = lemin->solution;
	while (tmp != NULL)
	{
		if (ft_strcmp((char *)tmp->name, name) == 0)
			return (1);
		tmp = tmp->next;
	}
	tmp = lemin->stack;
	while (tmp != NULL)
	{
		if (ft_strcmp((char *)tmp->name, name) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

char	*linked_room(t_list *link, char *room)
{
	if (link == NULL)
		return (NULL);
	if (ft_strcmp(((char **)(link->content))[0], room) == 0)
		return (((char **)(link->content))[1]);
	if (ft_strcmp(((char **)(link->content))[1], room) == 0)
		return (((char **)(link->content))[0]);
	return (NULL);
}

void	copy_room(t_lemin *lemin, t_room **child, char *name)
{
	t_room *search;

	new_room(child, name, 0, 0);
	search = lemin->rooms;
	while (ft_strcmp(search->name, name) != 0)
		search = search->next;
	(*child)->status = search->status;
}

void	children_of_room(t_lemin *lemin, t_room *room, t_room **children)
{
	t_list	*tmp;
	t_room	*child;
	char	*linked;

	tmp = lemin->links;
	while (tmp != NULL)
	{
		if ((linked = linked_room(tmp, room->name)) != NULL)
		{
			if (!solution_contains_room(lemin, linked))
			{
				copy_room(lemin, &child, linked);
				room_append(children, child);
			}
		}
		tmp = tmp->next;
	}
}

void	prepend(t_room **stack, t_room *to_add)
{
	t_room	*tmp;
	t_room	*prev;

	if (to_add == NULL)
		return ;
	tmp = to_add;
	while (tmp->next != NULL)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	tmp->next = *stack;
	*stack = to_add;
}

t_room	*pop_from_stack(t_room **stack)
{
	t_room	*ret;

	if (*stack == NULL)
		return (NULL);
	ret = *stack;
	*stack = (*stack)->next;
	ret->next = NULL;
	return (ret);
}

t_room	*del_from_stack(t_room **stack)
{
	t_room	*ret;
	t_room	*tmp;

	if (*stack == NULL)
		return (NULL);
	ret = *stack;
	while (ret->next != NULL)
	{
		tmp = ret;
		ret = ret->next;
	}
	tmp->next = NULL;
	return (ret);
}

void	del_room(t_room **room)
{
	free((*room)->name);
	free(*room);
}

void	find_solution(t_lemin *lemin)
{
	t_room	*item;
	t_room	*tmp;
	int		c;

	lemin->stack = NULL;
	new_room(&(lemin->solution), lemin->rooms->name, 0, 0);
	children_of_room(lemin, lemin->rooms, &(lemin->stack));
	while (lemin->stack != NULL)
	{
		tmp = NULL;
		item = pop_from_stack(&(lemin->stack));
		room_append(&(lemin->solution), item);
		if (item->status == END)
			break;
		children_of_room(lemin, item, &tmp);
		if (tmp == NULL)
		{
			item = del_from_stack(&(lemin->solution));
			del_room(&item);
		}
		else
			prepend(&(lemin->stack), tmp);
	}
}
