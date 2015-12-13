#include "nv_terminal_menubar.h"
#include "nv_terminal_window.h"

#include <vte/vte.h>

#include <math.h>

void nv_terminal_menu_bar_class_init(NvTerminalMenuBarClass *menu_bar);
void nv_terminal_menu_bar_init(NvTerminalMenuBar *menu_bar);
void nv_terminal_menu_bar_finalize(GObject *gobject);

void nv_terminal_menu_bar_new_window(GtkMenuItem *item, NvTerminalMenuBar *menu_bar);
void nv_terminal_menu_bar_new_tab(GtkMenuItem *item, NvTerminalMenuBar *menu_bar);
void nv_terminal_menu_bar_quit(GtkMenuItem *item, NvTerminalMenuBar *menu_bar);

void nv_terminal_menu_bar_zoom_in(GtkMenuItem *item, NvTerminalMenuBar *menu_bar);
void nv_terminal_menu_bar_zoom_out(GtkMenuItem *item, NvTerminalMenuBar *menu_bar);
void nv_terminal_menu_bar_reset(GtkMenuItem *item, NvTerminalMenuBar *menu_bar);
void nv_terminal_menu_bar_select_font(GtkMenuItem *item, NvTerminalMenuBar *menu_bar);

void nv_terminal_menu_bar_about(GtkMenuItem *item, NvTerminalMenuBar *menu_bar);

static gpointer nv_terminal_menu_bar_parent_class = NULL;

GType
nv_terminal_menu_bar_get_type()
{
  static GType nv_type_terminal_menu_bar = 0;

  if(!nv_type_terminal_menu_bar){
    static const GTypeInfo nv_terminal_menu_bar_info = {
      sizeof (NvTerminalMenuBarClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) nv_terminal_menu_bar_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (NvTerminalMenuBar),
      0,    /* n_preallocs */
      (GInstanceInitFunc) nv_terminal_menu_bar_init,
    };

    nv_type_terminal_menu_bar = g_type_register_static(GTK_TYPE_MENU_BAR,
						       "NvTerminalMenuBar\0", &nv_terminal_menu_bar_info,
						       0);
  }

  return(nv_type_terminal_menu_bar);
}

void
nv_terminal_menu_bar_class_init(NvTerminalMenuBarClass *menu_bar)
{
  GObjectClass *gobject;

  nv_terminal_menu_bar_parent_class = g_type_class_peek_parent(menu_bar);

  /* GObjectClass */
  gobject = (GObjectClass *) menu_bar;

  gobject->finalize = nv_terminal_menu_bar_finalize;
}

void
nv_terminal_menu_bar_init(NvTerminalMenuBar *menu_bar)
{
  GtkMenuItem *item;
  GtkMenu *menu;

  /* file menu */
  item = (GtkMenuItem *) gtk_menu_item_new_with_label("File\0");
  gtk_menu_bar_append(GTK_MENU_BAR(menu_bar),
		      GTK_WIDGET(item));

  menu = (GtkMenu *) gtk_menu_new();
  gtk_menu_item_set_submenu(item,
			    GTK_WIDGET(menu));

  item = (GtkMenuItem *) gtk_menu_item_new_with_label("New window\0");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			GTK_WIDGET(item));
  g_signal_connect(G_OBJECT(item), "activate\0",
		   G_CALLBACK(nv_terminal_menu_bar_new_window), menu_bar);

  item = (GtkMenuItem *) gtk_menu_item_new_with_label("New tab\0");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			GTK_WIDGET(item));
  g_signal_connect(G_OBJECT(item), "activate\0",
		   G_CALLBACK(nv_terminal_menu_bar_new_tab), menu_bar);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			GTK_WIDGET(gtk_separator_menu_item_new()));

  item = (GtkMenuItem *) gtk_menu_item_new_with_label("Quit\0");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			GTK_WIDGET(item));
  g_signal_connect(G_OBJECT(item), "activate\0",
		   G_CALLBACK(nv_terminal_menu_bar_quit), menu_bar);

  /* view menu */
  item = (GtkMenuItem *) gtk_menu_item_new_with_label("View\0");
  gtk_menu_bar_append(GTK_MENU_BAR(menu_bar),
		      GTK_WIDGET(item));

  menu = (GtkMenu *) gtk_menu_new();
  gtk_menu_item_set_submenu(item,
			    GTK_WIDGET(menu));

  item = (GtkMenuItem *) gtk_menu_item_new_with_label("Zoom in\0");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			GTK_WIDGET(item));
  g_signal_connect(G_OBJECT(item), "activate\0",
		   G_CALLBACK(nv_terminal_menu_bar_zoom_in), menu_bar);

  item = (GtkMenuItem *) gtk_menu_item_new_with_label("Zoom out\0");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			GTK_WIDGET(item));
  g_signal_connect(G_OBJECT(item), "activate\0",
		   G_CALLBACK(nv_terminal_menu_bar_zoom_out), menu_bar);

  item = (GtkMenuItem *) gtk_menu_item_new_with_label("Reset terminal\0");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			GTK_WIDGET(item));
  g_signal_connect(G_OBJECT(item), "activate\0",
		   G_CALLBACK(nv_terminal_menu_bar_reset), menu_bar);

  item = (GtkMenuItem *) gtk_menu_item_new_with_label("Select font\0");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			GTK_WIDGET(item));
  g_signal_connect(G_OBJECT(item), "activate\0",
		   G_CALLBACK(nv_terminal_menu_bar_select_font), menu_bar);

  /* help menu */
  item = (GtkMenuItem *) gtk_menu_item_new_with_label("Help\0");
  gtk_menu_bar_append(GTK_MENU_BAR(menu_bar),
		      GTK_WIDGET(item));

  menu = (GtkMenu *) gtk_menu_new();
  gtk_menu_item_set_submenu(item,
			    GTK_WIDGET(menu));

  item = (GtkMenuItem *) gtk_menu_item_new_with_label("About\0");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			GTK_WIDGET(item));
  g_signal_connect(G_OBJECT(item), "activate\0",
		   G_CALLBACK(nv_terminal_menu_bar_about), menu_bar);
}

void
nv_terminal_menu_bar_finalize(GObject *gobject)
{
  G_OBJECT_CLASS(nv_terminal_menu_bar_parent_class)->finalize(gobject);
}

void
nv_terminal_menu_bar_new_window(GtkMenuItem *item, NvTerminalMenuBar *menu_bar)
{
  GPid child_pid;
  GError *error;
  static gchar *argv[] = {
    "./nv_terminal\0",
    NULL,
  };

  error = NULL;
  g_spawn_async(NULL,
		argv,
		NULL,
		0,
		NULL,
		NULL,
		&child_pid,
		&error);
}

void
nv_terminal_menu_bar_new_tab(GtkMenuItem *item, NvTerminalMenuBar *menu_bar)
{
  NvTerminalWindow *window;

  window = gtk_widget_get_ancestor(GTK_WIDGET(menu_bar),
				   NV_TYPE_TERMINAL_WINDOW);

  nv_terminal_window_new_tab(window);
  gtk_widget_show_all(GTK_WIDGET(window));
}

void
nv_terminal_menu_bar_quit(GtkMenuItem *item, NvTerminalMenuBar *menu_bar)
{
  gtk_main_quit();
}

void
nv_terminal_menu_bar_zoom_in(GtkMenuItem *item, NvTerminalMenuBar *menu_bar)
{
  NvTerminalWindow *window;
  PangoFontDescription *font_desc, *new_desc;
  GList *list;
  double scale = exp(M_E * log(0.9));
  double preset;

  window = gtk_widget_get_ancestor(GTK_WIDGET(menu_bar),
				   NV_TYPE_TERMINAL_WINDOW);

  list = gtk_container_get_children(GTK_CONTAINER(window->notebook));

  while(list != NULL){
    g_object_get(VTE_TERMINAL(list->data),
		 "font-desc\0", &font_desc,
		 NULL);

    new_desc = pango_font_description_copy(font_desc);
    preset = (double) pango_font_description_get_size(new_desc);

    pango_font_description_set_size(new_desc,
				    preset * scale);
    g_object_set(VTE_TERMINAL(list->data),
		 "font-desc\0", new_desc,
		 NULL);

    gdk_window_invalidate_rect(gtk_widget_get_root_window(list->data),
			       NULL,
			       TRUE);
    gtk_widget_queue_draw(GTK_WIDGET(list->data));

    list = list->next;
  }
}

void
nv_terminal_menu_bar_zoom_out(GtkMenuItem *item, NvTerminalMenuBar *menu_bar)
{
  NvTerminalWindow *window;
  PangoFontDescription *font_desc, *new_desc;
  GList *list;
  double scale = exp(M_E * log(1.1));
  double preset;

  window = gtk_widget_get_ancestor(GTK_WIDGET(menu_bar),
				   NV_TYPE_TERMINAL_WINDOW);

  list = gtk_container_get_children(GTK_CONTAINER(window->notebook));

  while(list != NULL){
    g_object_get(VTE_TERMINAL(list->data),
		 "font-desc\0", &font_desc,
		 NULL);

    new_desc = pango_font_description_copy(font_desc);
    preset = (double) pango_font_description_get_size(new_desc);

    pango_font_description_set_size(new_desc,
				    preset * scale);
    g_object_set(VTE_TERMINAL(list->data),
		 "font-desc\0", new_desc,
		 NULL);

    gdk_window_invalidate_rect(gtk_widget_get_window(list->data),
			       NULL,
			       TRUE);
    gtk_widget_queue_draw(GTK_WIDGET(list->data));

    list = list->next;
  }
}

void
nv_terminal_menu_bar_reset(GtkMenuItem *item, NvTerminalMenuBar *menu_bar)
{
}

void
nv_terminal_menu_bar_select_font(GtkMenuItem *item, NvTerminalMenuBar *menu_bar)
{
  NvTerminalWindow *window;
  GtkFontSelectionDialog *dialog;
  gchar *font;
  GList *list;
  gint response;

  dialog = gtk_font_selection_dialog_new("Select font\0");

  response = gtk_dialog_run(GTK_DIALOG(dialog));

  if(response == GTK_RESPONSE_OK){
    font = gtk_font_selection_dialog_get_font_name(dialog);
    
    if(font != NULL){
      window = gtk_widget_get_ancestor(GTK_WIDGET(menu_bar),
				       NV_TYPE_TERMINAL_WINDOW);
      list = gtk_container_get_children(GTK_CONTAINER(window->notebook));

      while(list != NULL){
	vte_terminal_set_font_from_string_full(VTE_TERMINAL(list->data),
					       font,
					       VTE_ANTI_ALIAS_FORCE_ENABLE);
	list = list->next;
      }
    }
  }

  gtk_widget_destroy(GTK_WIDGET(dialog));
}

void
nv_terminal_menu_bar_about(GtkMenuItem *item, NvTerminalMenuBar *menu_bar)
{
  gchar *authors[] = { "Joël Krähemann\0", NULL }; 

  gtk_show_about_dialog((GtkWindow *) gtk_widget_get_ancestor((GtkWidget *) menu_bar, GTK_TYPE_WINDOW),
			"program-name\0", "nv-terminal\0",
			"authors\0", authors,
			"version\0", "0.1.0-SNAPSHOT\0",
			"title\0", "Nova Terminal\0",
			NULL);
}

NvTerminalMenuBar*
nv_terminal_menu_bar_new()
{
  NvTerminalMenuBar *menu_bar;

  menu_bar = (NvTerminalMenuBar *) g_object_new(NV_TYPE_TERMINAL_MENU_BAR,
						NULL);

  return(menu_bar);
}
