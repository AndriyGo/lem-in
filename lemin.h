#ifndef LEMIN_H
# define LEMIN_H
# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>

# define NONE 0
# define START 1
# define END 2

# define ANTS_N 0
# define ROOMS 1
# define LINKS 2

typedef struct		s_lemin
{
	t_list			*rooms;
	t_list			*links;
	int				ant_count;
	int				op;
	int				end_found;
	int				mode;
}					t_lemin;

void				init_lemin(t_lemin **lemin);
void				add_room(t_lemin *lemin, char *name);
void				add_link(t_lemin *lemin, char *r1, char *r2);
void				die(void);

#endif
