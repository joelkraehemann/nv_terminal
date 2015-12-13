#ifndef __NV_TERMINAL_EMULATOR_H__
#define __NV_TERMINAL_EMULATOR_H__

#include <glib.h>
#include <glib-object.h>

#define NV_TYPE_TERMINAL_EMULATOR                (nv_terminal_emulator_get_type())
#define NV_TERMINAL_EMULATOR(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), NV_TYPE_TERMINAL_EMULATOR, NvTerminalEmulator))
#define NV_TERMINAL_EMULATOR_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), NV_TYPE_TERMINAL_EMULATOR, NvTerminalEmulatorClass))
#define NV_IS_TERMINAL_EMULATOR(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), NV_TYPE_TERMINAL_EMULATOR))
#define NV_IS_TERMINAL_EMULATOR_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), NV_TYPE_TERMINAL_EMULATOR))
#define NV_TERMINAL_EMULATOR_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), NV_TYPE_TERMINAL_EMULATOR, NvTerminalEmulatorClass))

typedef struct _NvTerminalEmulator NvTerminalEmulator;
typedef struct _NvTerminalEmulatorClass NvTerminalEmulatorClass;

struct _NvTerminalEmulator
{
  GObject gobject;
};

struct _NvTerminalEmulatorClass
{
  GObjectClass gobject;
};

GType nv_terminal_emulator_get_type(void);

void nv_terminal_emulator_poll(NvTerminalEmulator *emulator);

NvTerminalEmulator* nv_terminal_emulator_new();

#endif /*__NV_TERMINAL_EMULATOR_H__*/
