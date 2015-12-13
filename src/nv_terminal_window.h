#ifndef __NV_TERMINAL_WINDOW_H__
#define __NV_TERMINAL_WINDOW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include "nv_terminal_menubar.h"

#define NV_TYPE_TERMINAL_WINDOW                (nv_terminal_window_get_type())
#define NV_TERMINAL_WINDOW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), NV_TYPE_TERMINAL_WINDOW, NvTerminalWindow))
#define NV_TERMINAL_WINDOW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), NV_TYPE_TERMINAL_WINDOW, NvTerminalWindowClass))
#define NV_IS_TERMINAL_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), NV_TYPE_TERMINAL_WINDOW))
#define NV_IS_TERMINAL_WINDOW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), NV_TYPE_TERMINAL_WINDOW))
#define NV_TERMINAL_WINDOW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), NV_TYPE_TERMINAL_WINDOW, NvTerminalWindowClass))

typedef struct _NvTerminalWindow NvTerminalWindow;
typedef struct _NvTerminalWindowClass NvTerminalWindowClass;

struct _NvTerminalWindow
{
  GtkWindow window;

  NvTerminalMenuBar *menu_bar;

  GtkNotebook *notebook;
};

struct _NvTerminalWindowClass
{
  GtkWindowClass window;
};

GType nv_terminal_window_get_type(void);

void nv_terminal_window_new_tab(NvTerminalWindow *window);

NvTerminalWindow* nv_terminal_window_new();

#endif /*__NV_TERMINAL_WINDOW_H__*/
