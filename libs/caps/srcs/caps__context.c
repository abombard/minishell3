/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 caps__context.c									:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: abombard <marvin@42.fr>					+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2016/04/01 16:28:06 by abombard		   #+#	  #+#			  */
/*	 Updated: 2016/04/01 19:30:17 by abombard		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "internal_caps.h"
#include "types.h"
#include "log.h"

# define MAP_ENTRY(d,c,k,f) {.description=d, .tcapcode=c, .keycode=k, .func=f}

# define TCAPCODE_INIT(desc,tcap) MAP_ENTRY(desc,tcap,BUFFER_INIT(0,NULL),NULL)
# define KEYCODE_INIT(desc,keycode)	  MAP_ENTRY(desc,NULL,(keycode),NULL)

static t_internal_context g_internal_context = {
	.termtype = NULL,
	.buffaddr = NULL,
	.map_size = 0,
	.map = {
		KEYCODE_INIT("meta space", CAPS__KEYCODE_META_SPACE),
		KEYCODE_INIT("meta a", CAPS__KEYCODE_META_A),
		KEYCODE_INIT("meta b", CAPS__KEYCODE_META_B),
		KEYCODE_INIT("meta c", CAPS__KEYCODE_META_C),
		KEYCODE_INIT("meta d", CAPS__KEYCODE_META_D),
		KEYCODE_INIT("meta e", CAPS__KEYCODE_META_E),
		KEYCODE_INIT("meta f", CAPS__KEYCODE_META_F),
		KEYCODE_INIT("meta g", CAPS__KEYCODE_META_G),
		KEYCODE_INIT("meta h", CAPS__KEYCODE_META_H),
		KEYCODE_INIT("meta i", CAPS__KEYCODE_META_I),
		KEYCODE_INIT("meta j", CAPS__KEYCODE_META_J),
		KEYCODE_INIT("meta k", CAPS__KEYCODE_META_K),
		KEYCODE_INIT("meta l", CAPS__KEYCODE_META_L),
		KEYCODE_INIT("meta m / enter", CAPS__KEYCODE_META_M),
		KEYCODE_INIT("meta n", CAPS__KEYCODE_META_N),
		KEYCODE_INIT("meta o", CAPS__KEYCODE_META_O),
		KEYCODE_INIT("meta p", CAPS__KEYCODE_META_P),
		KEYCODE_INIT("meta q", CAPS__KEYCODE_META_Q),
		KEYCODE_INIT("meta r", CAPS__KEYCODE_META_R),
		KEYCODE_INIT("meta s", CAPS__KEYCODE_META_S),
		KEYCODE_INIT("meta t", CAPS__KEYCODE_META_T),
		KEYCODE_INIT("meta u", CAPS__KEYCODE_META_U),
		KEYCODE_INIT("meta v", CAPS__KEYCODE_META_V),
		KEYCODE_INIT("meta w", CAPS__KEYCODE_META_W),
		KEYCODE_INIT("meta x", CAPS__KEYCODE_META_X),
		KEYCODE_INIT("meta y", CAPS__KEYCODE_META_Y),
		KEYCODE_INIT("meta z", CAPS__KEYCODE_META_Z),
		KEYCODE_INIT("meta cursor left key", CAPS__KEYCODE_META_CURSOR_LEFT),
		KEYCODE_INIT("meta cursor right key", CAPS__KEYCODE_META_CURSOR_RIGHT),
		KEYCODE_INIT("meta cursor up key", CAPS__KEYCODE_META_CURSOR_UP),
		KEYCODE_INIT("meta cursor down key", CAPS__KEYCODE_META_CURSOR_DOWN),
		KEYCODE_INIT("escape key", CAPS__KEYCODE_ESCAPE),
		KEYCODE_INIT("backspace key", CAPS__KEYCODE_BACKSPACE),
		TCAPCODE_INIT("shifted save key", "!1"),
		TCAPCODE_INIT("shifted suspend key", "!2"),
		TCAPCODE_INIT("shifted undo key", "!3"),
		TCAPCODE_INIT("shifted help key", "#1"),
		TCAPCODE_INIT("shifted home key", "#2"),
		TCAPCODE_INIT("shifted input key", "#3"),
		TCAPCODE_INIT("shifted cursor left key", "#4"),
		TCAPCODE_INIT("redo key", "%0"),
		TCAPCODE_INIT("help key", "%1"),
		TCAPCODE_INIT("mark key", "%2"),
		TCAPCODE_INIT("message key", "%3"),
		TCAPCODE_INIT("move key", "%4"),
		TCAPCODE_INIT("next-object key", "%5"),
		TCAPCODE_INIT("open key", "%6"),
		TCAPCODE_INIT("options key", "%7"),
		TCAPCODE_INIT("previous-object key", "%8"),
		TCAPCODE_INIT("print key", "%9"),
		TCAPCODE_INIT("shifted message key", "%a"),
		TCAPCODE_INIT("shifted move key", "%b"),
		TCAPCODE_INIT("shifted next key", "%c"),
		TCAPCODE_INIT("shifted options key", "%d"),
		TCAPCODE_INIT("shifted previous key", "%e"),
		TCAPCODE_INIT("shifted print key", "%f"),
		TCAPCODE_INIT("shifted redo key", "%g"),
		TCAPCODE_INIT("shifted replace key", "%h"),
		TCAPCODE_INIT("shifted cursor right key", "%i"),
		TCAPCODE_INIT("shifted resume key", "%j"),
		TCAPCODE_INIT("shifted cancel key", "&0"),
		TCAPCODE_INIT("reference key", "&1"),
		TCAPCODE_INIT("refresh key", "&2"),
		TCAPCODE_INIT("replace key", "&3"),
		TCAPCODE_INIT("restart key", "&4"),
		TCAPCODE_INIT("resume key", "&5"),
		TCAPCODE_INIT("save key", "&6"),
		TCAPCODE_INIT("suspend key", "&7"),
		TCAPCODE_INIT("undo key", "&8"),
		TCAPCODE_INIT("shifted begin key", "&9"),
		TCAPCODE_INIT("shifted find key", "*0"),
		TCAPCODE_INIT("shifted command key", "*1"),
		TCAPCODE_INIT("shifted copy key", "*2"),
		TCAPCODE_INIT("shifted create key", "*3"),
		TCAPCODE_INIT("select key", "*6"),
		TCAPCODE_INIT("shifted end key", "*7"),
		TCAPCODE_INIT("shifted clear line key", "*8"),
		TCAPCODE_INIT("shifted exit key", "*9"),
		TCAPCODE_INIT("find key", "@0"),
		TCAPCODE_INIT("begin key", "@1"),
		TCAPCODE_INIT("cancel key", "@2"),
		TCAPCODE_INIT("close key", "@3"),
		TCAPCODE_INIT("command key", "@4"),
		TCAPCODE_INIT("copy key", "@5"),
		TCAPCODE_INIT("create key", "@6"),
		TCAPCODE_INIT("end key", "@7"),
		TCAPCODE_INIT("enter/send key", "@8"),
		TCAPCODE_INIT("exit key", "@9"),
		TCAPCODE_INIT("The string sent by function key f11", "F1"),
		TCAPCODE_INIT("The string sent by function key f12", "F2"),
		TCAPCODE_INIT("The string sent by function key f13", "F3"),
		TCAPCODE_INIT("The string sent by function key f19", "F9"),
		TCAPCODE_INIT("The string sent by function key f20", "FA"),
		TCAPCODE_INIT("The string sent by function key f21", "FB"),
		TCAPCODE_INIT("The string sent by function key f45", "FZ"),
		TCAPCODE_INIT("The string sent by function key f46", "Fa"),
		TCAPCODE_INIT("The string sent by function key f47", "Fb"),
		TCAPCODE_INIT("The string sent by function key f63", "Fr"),
		TCAPCODE_INIT("upper left key on keypad", "K1"),
		TCAPCODE_INIT("center key on keypad", "K2"),
		TCAPCODE_INIT("upper right key on keypad", "K3"),
		TCAPCODE_INIT("bottom left key on keypad", "K4"),
		TCAPCODE_INIT("bottom right key on keypad", "K5"),
		TCAPCODE_INIT("Function key 0", "k0"),
		TCAPCODE_INIT("Function key 1", "k1"),
		TCAPCODE_INIT("Function key 2", "k2"),
		TCAPCODE_INIT("Function key 3", "k3"),
		TCAPCODE_INIT("Function key 4", "k4"),
		TCAPCODE_INIT("Function key 5", "k5"),
		TCAPCODE_INIT("Function key 6", "k6"),
		TCAPCODE_INIT("Function key 7", "k7"),
		TCAPCODE_INIT("Function key 8", "k8"),
		TCAPCODE_INIT("Function key 9", "k9"),
		TCAPCODE_INIT("Function key 10", "k,"),
		TCAPCODE_INIT("Clear all tabs key", "ka"),
		TCAPCODE_INIT("Insert line key", "kA"),
		TCAPCODE_INIT("Clear screen key", "kC"),
		TCAPCODE_INIT("Cursor down key", "kd"),
		TCAPCODE_INIT("Key for delete character under cursor", "kD"),
		TCAPCODE_INIT("turn keypad off", "ke"),
		TCAPCODE_INIT("Key for clear to end of line", "kE"),
		TCAPCODE_INIT("Key for scrolling forward/down", "kF"),
		TCAPCODE_INIT("Cursor home key", "kh"),
		TCAPCODE_INIT("Cursor hown down key", "kH"),
		TCAPCODE_INIT("Insert character/Insert mode key", "kI"),
		TCAPCODE_INIT("Cursor left key", "kl"),
		TCAPCODE_INIT("Key for delete line", "kL"),
		TCAPCODE_INIT("Key for exit insert mode", "kM"),
		TCAPCODE_INIT("Key for next page", "kN"),
		TCAPCODE_INIT("Key for previous page", "kP"),
		TCAPCODE_INIT("Cursor right key", "kr"),
		TCAPCODE_INIT("Key for scrolling backward/up", "kR"),
		TCAPCODE_INIT("Turn keypad on", "ks"),
		TCAPCODE_INIT("Clear to end of screen key", "kS"),
		TCAPCODE_INIT("Clear this tab key", "kt"),
		TCAPCODE_INIT("Set tab here key", "kT"),
		TCAPCODE_INIT("Cursor up key", "ku"),
		TCAPCODE_INIT("Label of zeroth function key, if not f0", "l0"),
		TCAPCODE_INIT("Label of first function key, if not f1", "l1"),
		TCAPCODE_INIT("Label of first function key, if not f2", "l2"),
		TCAPCODE_INIT("Label of tenth function key, if not f10", "la"),
		TCAPCODE_INIT("Program key %1 to send str %2 as if typed by user", "pk"),
		TCAPCODE_INIT("Program key %1 to execute string %2 in local mode", "pl"),
		TCAPCODE_INIT("Program key %1 to send string %2 to computer", "px"),
	}
};

void	caps__get_context(t_internal_context **out_context)
{
	*out_context = &g_internal_context;
}
