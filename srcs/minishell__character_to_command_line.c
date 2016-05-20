#include "minishell.h"
#include "types.h"
#include "log.h"

/*
** Add a character to the current command line
*/
bool	minishell__character_to_command_line(const size_t character_bytes_count,
											 const char *character_bytes,
											 t_list_head *command_line)
{
	t_list_node_cmd	*new;

	new = list_node__command_line_create(character_bytes_count,
										 character_bytes);
	if (!new)
		FATAL("list_node__command_line_create() failed bytes_count %zu", character_bytes_count);
	list_head__insert(command_line,
					  command_line->offset,
					  &new->list);
	return (TRUE);
}
