#include "nv_terminal_window.h"

#include <pty.h>
#include <vte/vte.h>

void nv_terminal_window_class_init(NvTerminalWindowClass *window);
void nv_terminal_window_init(NvTerminalWindow *window);
void nv_terminal_window_finalize(GObject *gobject);

void nv_terminal_window_delete_event(GtkWidget *widget,
				     GdkEvent *event,
				     gpointer data);

static gpointer nv_terminal_window_parent_class = NULL;

GType
nv_terminal_window_get_type()
{
  static GType nv_type_terminal_window = 0;

  if(!nv_type_terminal_window){
    static const GTypeInfo nv_terminal_window_info = {
      sizeof (NvTerminalWindowClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) nv_terminal_window_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (NvTerminalWindow),
      0,    /* n_preallocs */
      (GInstanceInitFunc) nv_terminal_window_init,
    };

    nv_type_terminal_window = g_type_register_static(GTK_TYPE_WINDOW,
						     "NvTerminalWindow\0", &nv_terminal_window_info,
						     0);
  }

  return(nv_type_terminal_window);
}

void
nv_terminal_window_class_init(NvTerminalWindowClass *window)
{
  GObjectClass *gobject;

  nv_terminal_window_parent_class = g_type_class_peek_parent(window);

  /* GObjectClass */
  gobject = (GObjectClass *) window;

  gobject->finalize = nv_terminal_window_finalize;
}

void
nv_terminal_window_init(NvTerminalWindow *window)
{
  GtkVBox *vbox;

  g_signal_connect_after(G_OBJECT(window), "delete-event\0",
			 G_CALLBACK(nv_terminal_window_delete_event), NULL);

  vbox = (GtkVBox *) gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window),
		    GTK_WIDGET(vbox));

  window->menu_bar = (NvTerminalMenuBar *) nv_terminal_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(vbox),
		     GTK_WIDGET(window->menu_bar),
		     FALSE, TRUE,
		     0);

  window->notebook = (GtkNotebook *) gtk_notebook_new();
  gtk_box_pack_start(GTK_BOX(vbox),
		     GTK_WIDGET(window->notebook),
		     TRUE, TRUE,
		     0);
}

void
nv_terminal_window_finalize(GObject *gobject)
{
  G_OBJECT_CLASS(nv_terminal_window_parent_class)->finalize(gobject);
}

void
nv_terminal_window_new_tab(NvTerminalWindow *window)
{
  VteTerminal *terminal;
  VtePty *pty;
  GPid pid;
  int amaster, aslave;
  char name[64];
  GError *error;
  static GdkColor black, white;
  static initialized = FALSE;

  terminal = (VteTerminal *) vte_terminal_new();
  vte_terminal_set_scrollback_lines(VTE_TERMINAL (terminal), -1); /* infinite scrollback */
  gtk_notebook_append_page(window->notebook,
			   (GtkWidget *) terminal,
			   gtk_label_new("bash\0"));

  if(!initialized){
    gdk_color_black(gtk_widget_get_colormap(GTK_WIDGET(terminal)),
		    &black);
    gdk_color_white(gtk_widget_get_colormap(GTK_WIDGET(terminal)),
		    &white);

    initialized = TRUE;
  }

  vte_terminal_set_colors(terminal,
			  &black,
			  &white,
			  NULL,
			  0);

  openpty(&amaster, &aslave, name, NULL, NULL);
  vte_terminal_set_pty(terminal,
		       amaster);

  error = NULL;
  pty = (VtePty *) vte_pty_new(0,
			       &error);

  if(error != NULL){
    g_warning(error->message);
  }else{
    const char *argv[] = {
      "./nv_emulator\0",
      NULL,
    };
    const char *envv[] = {
      NULL,
    };

    vte_terminal_set_pty_object(terminal,
				pty);
    vte_pty_child_setup(pty);

    error = NULL;
    vte_terminal_fork_command_full(terminal,
				   VTE_PTY_DEFAULT,
				   NULL,
				   argv,
				   envv,
				   0,
				   NULL,
				   NULL,
				   &pid,
				   &error);

    if(error != NULL){
      g_warning(error->message);
    }

    if(pid == 0){
      _exit(0);
    }else if(pid > 0){
      g_message("continue");
    }else{
      g_warning("failed to fork");
    }
  }
}

void
nv_terminal_window_delete_event(GtkWidget *widget,
				GdkEvent *event,
				gpointer data)
{
  gtk_main_quit();
}

NvTerminalWindow*
nv_terminal_window_new()
{
  NvTerminalWindow *window;

  window = (NvTerminalWindow *) g_object_new(NV_TYPE_TERMINAL_WINDOW,
					     NULL);

  return(window);
}
