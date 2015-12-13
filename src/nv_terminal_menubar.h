#ifndef __NV_TERMINAL_MENUBAR_H__
#define __NV_TERMINAL_MENUBAR_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define NV_TYPE_TERMINAL_MENU_BAR                (nv_terminal_menu_bar_get_type())
#define NV_TERMINAL_MENU_BAR(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), NV_TYPE_TERMINAL_MENU_BAR, NvTerminalMenuBar))
#define NV_TERMINAL_MENU_BAR_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), NV_TYPE_TERMINAL_MENU_BAR, NvTerminalMenuBarClass))
#define NV_IS_TERMINAL_MENU_BAR(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), NV_TYPE_TERMINAL_MENU_BAR))
#define NV_IS_TERMINAL_MENU_BAR_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), NV_TYPE_TERMINAL_MENU_BAR))
#define NV_TERMINAL_MENU_BAR_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), NV_TYPE_TERMINAL_MENU_BAR, NvTerminalMenuBarClass))

typedef struct _NvTerminalMenuBar NvTerminalMenuBar;
typedef struct _NvTerminalMenuBarClass NvTerminalMenuBarClass;

struct _NvTerminalMenuBar
{
  GtkMenuBar menu_bar;
};

struct _NvTerminalMenuBarClass
{
  GtkMenuBarClass menu_bar;
};

GType nv_terminal_menu_bar_get_type(void);

NvTerminalMenuBar* nv_terminal_menu_bar_new();

#endif /*__NV_TERMINAL_MENUBAR_H__*/
