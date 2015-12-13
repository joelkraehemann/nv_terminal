#include "nv_terminal_window.h"

int
main(int argc, char **argv)
{
  NvTerminalWindow *terminal_window;

  gtk_init(&argc, &argv);
  gtk_rc_parse("./style.rc");

  terminal_window = nv_terminal_window_new();
  nv_terminal_window_new_tab(terminal_window);
  gtk_widget_show_all(GTK_WIDGET(terminal_window));

  gtk_main();

  return(0);
}
