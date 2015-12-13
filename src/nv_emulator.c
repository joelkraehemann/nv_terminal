#include "nv_terminal_emulator.h"

int
main(int argc, char **argv)
{
  NvTerminalEmulator *terminal_emulator;

  terminal_emulator = nv_terminal_emulator_new();
  nv_terminal_emulator_poll(terminal_emulator);
  g_object_unref(terminal_emulator);

  return(0);
}
