#include "nv_terminal_emulator.h"

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <term.h>
#include <termios.h>

void nv_terminal_emulator_class_init(NvTerminalEmulatorClass *emulator);
void nv_terminal_emulator_init(NvTerminalEmulator *emulator);
void nv_terminal_emulator_finalize(GObject *gobject);

gchar* nv_terminal_emulator_readline(NvTerminalEmulator *emulator);
gchar* nv_terminal_emulator_unescape(NvTerminalEmulator *emulator,
				     gchar *str);
const gchar* nv_terminal_emulator_lookup(NvTerminalEmulator *emulator,
					 gchar *str,
					 gboolean suid_path);

static gpointer nv_terminal_emulator_parent_class = NULL;

GType
nv_terminal_emulator_get_type()
{
  static GType nv_type_terminal_emulator = 0;

  if(!nv_type_terminal_emulator){
    static const GTypeInfo nv_terminal_emulator_info = {
      sizeof (NvTerminalEmulatorClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) nv_terminal_emulator_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (NvTerminalEmulator),
      0,    /* n_preallocs */
      (GInstanceInitFunc) nv_terminal_emulator_init,
    };

    nv_type_terminal_emulator = g_type_register_static(G_TYPE_OBJECT,
						       "NvTerminalEmulator\0", &nv_terminal_emulator_info,
						       0);
  }

  return(nv_type_terminal_emulator);
}

void
nv_terminal_emulator_class_init(NvTerminalEmulatorClass *emulator)
{
  GObjectClass *gobject;

  nv_terminal_emulator_parent_class = g_type_class_peek_parent(emulator);

  /* GObjectClass */
  gobject = (GObjectClass *) emulator;

  gobject->finalize = nv_terminal_emulator_finalize;
}

void
nv_terminal_emulator_init(NvTerminalEmulator *emulator)
{
}

void
nv_terminal_emulator_finalize(GObject *gobject)
{
  G_OBJECT_CLASS(nv_terminal_emulator_parent_class)->finalize(gobject);
}

gchar*
nv_terminal_emulator_readline(NvTerminalEmulator *emulator)
{
  gchar *str;
  gchar *buffer, *iter;
  gchar c;
  unsigned int escapes;
  gboolean control_sequence;

  static const unsigned int buffer_size = 256;

  str = NULL;
  buffer = (gchar *) malloc(buffer_size * sizeof(gchar));
  memset(buffer, 0, buffer_size * sizeof(gchar));

  iter = buffer;
  escapes = 0;

 nv_terminal_emulator_readlineREPEAT:

  while((c = getchar()) != '\n'){
    control_sequence = FALSE;

    if(control_sequence){
      continue;
    }

    *iter = c;
    
    if(c == '\\'){
      escapes++;
    }else{
      escapes = 0;
    }

    iter++;

    if(iter == &(buffer[buffer_size - 1])){
      *iter = '\0';
	
      if(str == NULL){
	str = g_strdup(buffer);
      }else{
	str = g_strconcat(str,
			  buffer,
			  NULL);
      }

      iter = buffer;
      memset(buffer, 0, buffer_size * sizeof(gchar));
    }
  }

  if(iter != buffer){
    *iter = '\0';

    if(str == NULL){
      str = g_strdup(buffer);
    }else{
      str = g_strconcat(str,
			buffer,
			NULL);
    }
  }

  if(escapes % 2 == 1){
    escapes = 0;

    if(str == NULL){
      str = g_strdup("\n\0");
    }else{
      str = g_strconcat(str,
			"\n\0",
			NULL);
    }

    iter = buffer;
    memset(buffer, 0, buffer_size * sizeof(gchar));

    goto nv_terminal_emulator_readlineREPEAT;
  }

  return(str);
}

gchar*
nv_terminal_emulator_unescape(NvTerminalEmulator *emulator,
			      gchar *str)
{
  str = g_strcompress(str);

  return(str);
}

const gchar*
nv_terminal_emulator_lookup(NvTerminalEmulator *emulator,
			    gchar *str,
			    gboolean suid_path)
{
  gchar **path;

  auto gboolean nv_terminal_emulator_lookup_path(gchar *path);

  static const gchar *search_path[] = {
    "/bin\0",
    "/usr/bin\0",
    "/usr/X11/bin\0",
    "/opt/local/bin\0",
    NULL,
  };
  static const gchar *suid_search_path[] = {
    "/sbin\0",
    "/usr/sbin\0",
    NULL,
  };

  gboolean nv_terminal_emulator_lookup_path(gchar *path){
    DIR *directory;
    struct dirent *entry;

    directory = opendir(path);

    while((entry = readdir(directory)) != NULL){
      switch(entry->d_type){
      case DT_REG:
	{
	  if(!g_strcmp0(str, entry->d_name)){
	    closedir(directory);
	    return(TRUE);
	  }

	  break;
	}
      }
    }

    closedir(directory);

    return(FALSE);
  }

  if(suid_path){
    path = suid_search_path;
    
    for(; *path != NULL; path++){
      if(nv_terminal_emulator_lookup_path(*path)){
	return(*path);
      }
    }
  }

  path = search_path;

  for(; *path != NULL; path++){
    if(nv_terminal_emulator_lookup_path(*path)){
      return(*path);
    }
  }

  return(NULL);
}

void
nv_terminal_emulator_poll(NvTerminalEmulator *emulator)
{
  struct passwd *pw;
  uid_t uid;
  gchar *username;
  gchar *hostname;
  gchar *path;
  gchar *str;
  gchar **tokens;
  gboolean running;

  uid = getuid();
  pw = getpwuid(uid);

  username = pw->pw_name;
  hostname = (gchar *) malloc(256 * sizeof(gchar));
  gethostname(hostname, 256);

  path = (gchar *) malloc(256 * sizeof(gchar));

  running = TRUE;

  while(running){
    getcwd(path, 256);

    printf("%s@%s $ \0",
	   username,
	   hostname);

    str = nv_terminal_emulator_readline(emulator);

    if(str != NULL){
      gchar *filename;
      gchar *tmp;

      str = nv_terminal_emulator_unescape(emulator,
					  str);

      tokens = g_strsplit_set(str,
			      " \t\n\0",
			      0);
      g_free(str);

      if(!g_str_has_prefix(tokens[0],
			   "./\0")){
	tmp = nv_terminal_emulator_lookup(emulator,
					  tokens[0],
					  ((uid == 0) ? TRUE: FALSE));
      }else{
	tmp = g_strdup(tokens[0]);
      }

      if(tmp != NULL){
	filename = g_strconcat(tmp,
			       "/\0",
			       tokens[0],
			       NULL);

	if(filename != NULL){
	  pid_t pid;

	  pid = fork();

	  if(pid == 0){
	    execvp(filename,
		   tokens);
	    _exit(0);
	  }else if(pid > 0){
	    waitpid(pid,
		    WEXITED,
		    0);
	  }

	  g_free(filename);
	}
      }


      g_strfreev(tokens);
    }
  }
}

NvTerminalEmulator*
nv_terminal_emulator_new()
{
  NvTerminalEmulator *emulator;

  emulator = (NvTerminalEmulator *) g_object_new(NV_TYPE_TERMINAL_EMULATOR,
						 NULL);

  return(emulator);
}
