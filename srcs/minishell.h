#ifndef MINISHELL_H
# define MINISHELL_H

# include "list_head.h"

# define PROMPT_SIZE		(sizeof ("$> ") - 1)
# define PROMPT						 "$> "

# define MINISHELL_KEY__SHARE__SELECTION_OFFSET_UNDEFINED	(size_t)-1

/*
** Internal context
*/
typedef struct	s_internal_context
{
	enum { STATE_REGULAR,
		   STATE_SELECTION,
		   STATE_EXIT }						state;

	t_list_head								command_line;

	size_t									selection_offset_start;
	size_t									selection_offset_end;

	t_list_head								copy;

	t_list_head								history;
}				t_internal_context;

size_t	get_character_bytes_count(const size_t input_bytes_count,
								  const char *input_bytes,
								  size_t *out_missing_bytes_count);

bool	minishell__character_to_command_line(const size_t character_bytes_count,
											 const char *character_bytes,
											 t_list_head *command_line);
bool	minishell__string_to_command_line(const size_t input_buffer_size,
										  const char *input_buffer,
										  t_list_head *command_line);

bool	minishell__display_command_line(const t_list_head *command_line);

#endif /* MINISHELL_H */
