#include "internal_caps.h"
#include "termcap.h"
#include "log.h"

bool	caps__cursor_setxy(int x, int y)
{
	char	*cm;
	char	*cmd;

	cm = tgetstr("cm", NULL);
	if (!cm)
		FATAL("tgetstr() failed %s", "");
	cmd = tgoto(cm, y, x);
	if (!cmd)
		FATAL("tgoto() failed x %d y %d", x, y);
	caps__print(cmd, 0);
	return (TRUE);
}
