#include "lemin.h"

void	die(void)
{
	write(1, "ERROR\n", 6);
	exit(-1);
}

void	print_header(t_lemin *lemin)
{
	t_room	*room;
	t_list	*link;

	printf("%d\n", lemin->ant_count);
	room = lemin->rooms;
	while (room != NULL)
	{
		if (room->status == START)
			printf("##start\n");
		else if (room->status == END)
			printf("##end\n");
		printf("%s %d %d\n", room->name, room->x, room->y);
		room = room->next;
	}
	link = lemin->links;
	while (link != NULL)
	{
		printf("%s-%s\n", ((char **)link->content)[0], ((char **)link->content)[1]);
		link = link->next;
	}
	printf("\n");
}

void	print_move_line(t_lemin *lemin, int *step, int *min_ant, int *travelling)
{
	t_room	*tmp;
	int		j;
	int		i;
	int		end_reached;

	j = 0;
	end_reached = 0;
	while (j < *travelling)
	{
		i = 0;
		tmp = lemin->solution;
		while (i++ < *step - j)
			tmp = tmp->next;
		if (j > 0)
			printf(" ");
		printf("L%d-%s", j + *min_ant, tmp->name);
		if (tmp->next == NULL)
			end_reached = 1;
		j++;
	}
	if (end_reached || (lemin->ant_count == 0))
	{
		if (lemin->ant_count > 0)
			lemin->ant_count -= 1;
		else
			*travelling = *travelling - 1;
		*min_ant += (*travelling > 0);
	}
	else
	{
		*step = *step + 1;
		*travelling = *travelling + 1;
		lemin->ant_count -= 1;
	}
}

void	print_solution(t_lemin *lemin)
{
	int		step;
	int		min_ant;
	int		travelling;

	print_header(lemin);
	min_ant = 1;
	travelling = 1;
	lemin->ant_count -= travelling;
	step = 1;
	while (travelling > 0)
	{
		print_move_line(lemin, &step, &min_ant, &travelling);
		printf("\n");
	}
}