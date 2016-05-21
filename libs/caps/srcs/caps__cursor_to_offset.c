#include "internal_caps.h"

static void caps__cursor_to_line_offset(int current_line, const int dest_line)
{
	if (current_line > dest_line)
	{
		while (current_line > dest_line)
		{
			caps__print_cap(CAPS__UP, 0);
			current_line--;
		}
	}
	else if (current_line < dest_line)
	{
		while (current_line < dest_line)
		{
			caps__print_cap(CAPS__DOWN, 0);
			current_line++;
		}
	}
}

static void	caps__cursor_to_col_offset(int current_col, const int dest_col)
{
	if (current_col > dest_col)
	{
		while (current_col > dest_col)
		{
			caps__print_cap(CAPS__LEFT, 0);
			current_col--;
		}
	}
	else if (current_col < dest_col)
	{
		while (current_col < dest_col)
		{
			caps__print_cap(CAPS__RIGHT, 0);
			current_col++;
		}
	}
}

void		caps__cursor_to_offset(const size_t to_offset, const size_t current_offset)
{
	int		current_line;
	int		dest_line;
	int		current_col;
	int		dest_col;
	int		col_count;

	col_count = caps__win(WIN_COLUMNS);
	if (col_count != 0)
	{
		current_line = current_offset / col_count;
		dest_line = to_offset / col_count;
		current_col = current_offset % col_count;
		dest_col = to_offset % col_count;
		caps__cursor_to_line_offset(current_line, dest_line);
		caps__cursor_to_col_offset(current_col, dest_col);
	}
}
