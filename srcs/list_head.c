#include "list_head.h"
#include "types.h"
#include "log.h"
#include "libft.h"

#include <unistd.h>
#include <stdlib.h>

/*
** list head
*/
void	list_head__insert(t_list_head *head, size_t offset, t_list *new)
{
	list_insert(new, &head->list, offset);
	head->size += 1;
	head->offset += 1;
}

void	list_head__del(t_list_head *head, t_list *entry)
{
	list_del(entry);
	head->size -= 1;
}

void	list_head__init(t_list_head *head)
{
	head->size = 0;
	head->offset = 0;
	INIT_LIST_HEAD(&head->list);
}

void	list_head__slice(t_list_head *new, t_list_head *head, size_t start, size_t len)
{
	list_slice(&new->list, &head->list, start, len);
	new->size += len;
	head->size -= len;
}

/*
** list node command line
*/
t_list_node_cmd	*list_node__command_line_create(const size_t character_size, const char *character)
{
	t_list_node_cmd	*new;

	if (!character_size || character_size > CHARACTER_SIZE_MAX || !character)
	{
		LOG_ERROR("character_size %zu size_max %zu character %p",
				  character_size, CHARACTER_SIZE_MAX, (void *)character);
		return ((t_list_node_cmd *)0);
	}
	new = (t_list_node_cmd *)malloc(sizeof(t_list_node_cmd));
	if (!new)
	{
		LOG_ERROR("malloc() failed %s", "");
		return ((t_list_node_cmd *)0);
	}
	INIT_LIST_HEAD(&new->list);
	new->character_size = character_size;
	ft_memcpy(new->character, character, character_size);
	return (new);
}

void			list_node__command_line_destroy(t_list *entry)
{
	t_list_node_cmd	*node;

	node = CONTAINER_OF(entry, t_list_node_cmd, list);
	free(node);
}

/*
** list head command line
**
** dup()
** destroy()
** to buffer()
*/
t_list_head		*list_head__command_line_dup(t_list_head *dst, t_list_head *src)
{
	t_list			*pos;
	t_list_node_cmd	*cur;
	t_list_node_cmd	*new;
	size_t			offset;

	list_head__init(dst);
	offset = 0;
	LIST_FOREACH(&src->list, pos)
	{
		cur = CONTAINER_OF(pos, t_list_node_cmd, list);
		new = list_node__command_line_create(cur->character_size, cur->character);
		if (!new)
			FATAL("list_node__command_line_create() failed %s", "");
		list_head__insert(dst, offset, &new->list);
		offset++;
	}
	return (dst);
}

void			list_head__command_line_destroy(t_list_head *head)
{
	t_list			*pos;
	t_list			*pos_safe;
	t_list_node_cmd	*node_cmd;

	pos_safe = head->list.next;
	while ((pos = pos_safe) && pos != &head->list)
	{
		pos_safe = pos_safe->next;
		node_cmd = CONTAINER_OF(pos, t_list_node_cmd, list);
		free(node_cmd);
	}
}

# include "caps.h"
# define CAPS__NL_SIZE	(size_t)(sizeof("\n\r") - 1)
# define CAPS__NL						"\n\r"

bool		list_head__command_line_to_buffer(const t_list_head *head, const size_t buffer_size_max, size_t *buffer_size, char *buffer)
{
	size_t			buffer_offset;
	t_list			*pos;
	size_t			pos_offset;
	t_list_node_cmd *node_cmd;
	size_t			col_count = caps__win(WIN_COLUMNS);

	pos_offset = 0;
	buffer_offset = 0;
	LIST_FOREACH(&head->list, pos)
	{
		node_cmd = CONTAINER_OF(pos, t_list_node_cmd, list);
		if (buffer_offset + node_cmd->character_size > buffer_size_max)
		{
			buffer[buffer_offset - 1] = '$';
			break ;
		}
		ft_memcpy(buffer + buffer_offset, node_cmd->character, node_cmd->character_size);
		buffer_offset += node_cmd->character_size;
		pos_offset += 1;
		if (pos_offset % col_count == 0)
		{
			ft_memcpy(buffer + buffer_offset, CAPS__NL, CAPS__NL_SIZE);
			buffer_offset += CAPS__NL_SIZE;
		}
	}
	*buffer_size = buffer_offset;
	return (TRUE);
}

/*
** list node history
*/
t_list_node_history	*list_node__history_create(t_list_head *command_line)
{
	t_list_node_history	*new;

	if (!command_line)
	{
		LOG_ERROR("command_line %p", (void *)command_line);
		return (NULL);
	}
	new = (t_list_node_history *)malloc(sizeof(t_list_node_history));
	if (!new)
	{
		LOG_ERROR("malloc() failed %s", "");
		return (NULL);
	}
	new->command_line.size = command_line->size;
	new->command_line.offset = 0;
	INIT_LIST_HEAD(&new->list);
	INIT_LIST_HEAD(&new->command_line.list);
	list_splice(&command_line->list, &new->command_line.list);
	return (new);
}

/*
** list head history
*/
void			list_head__history_destroy(t_list_head *head)
{
	t_list				*pos;
	t_list				*pos_safe;
	t_list_node_history	*node_history;

	pos_safe = head->list.next;
	while ((pos = pos_safe) && pos != &head->list)
	{
		pos_safe = pos_safe->next;
		node_history = CONTAINER_OF(pos, t_list_node_history, list);
		list_head__command_line_destroy(&node_history->command_line);
		free(node_history);
	}
}
