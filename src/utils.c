/*
** utils.c for myftp in /home/rootkid/rendu/psu/PSU_2016_myftp
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Wed May 10 16:24:37 2017 theo champion
** Last update Sat May 20 12:20:54 2017 theo champion
*/

#include "header.h"

void		log_msg(int mode, const char *fmt, ...)
{
  va_list	ap;

  va_start(ap, fmt);
  if (mode == INFO)
    fprintf(stderr, "\033[34m[INFO] ");
  else if (mode == ERROR)
    fprintf(stderr, "\033[31m[ERROR] ");
  else
    fprintf(stderr, "\033[33m[DEBUG] ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n\033[0m");
  va_end(ap);
}

char	*resolve_path(t_handle *hdl, char *path)
{
  char	*fullpath;
  char	*resolved;

  if (!path)
    return (NULL);
  if (!(fullpath = malloc(sizeof(char) *
                          strlen(hdl->wd) + strlen(path) + 2)))
    return (NULL);
  sprintf(fullpath, "%s/%s", hdl->wd, path);
  resolved = realpath(fullpath, NULL);
  free(fullpath);
  if (!resolved)
    return (NULL);
  log_msg(DEBUG, "Resolved path \"%s\"", resolved);
  if (strlen(resolved) < strlen(hdl->home))
    {
      free(resolved);
      return (strdup(hdl->home));
    }
  else
    return (resolved);
}

FILE	*open_file(t_handle *hdl, char *filepath, char *mode)
{
  char	*fullpath;
  char	*dirc;
  char	*basec;
  char	*dname;
  char	*bname;
  FILE	*file;

  dirc = strdup(filepath);
  basec = strdup(filepath);
  dname = dirname(dirc);
  bname = basename(basec);
  file = NULL;
  if ((fullpath = resolve_path(hdl, dname)) != NULL)
    {
      fullpath = realloc(fullpath, sizeof(char) *
                         strlen(fullpath) + strlen(bname) + 2);
      sprintf(fullpath, "%s/%s", fullpath, bname);
      file = fopen(fullpath, mode);
      log_msg(DEBUG, "File to open is \"%s\"", fullpath);
    }
  free(fullpath);
  free(dirc);
  free(basec);
  return (file);
}

FILE	*open_cmd_stream(t_handle *hdl, char *arg)
{
  char	*fullpath;
  char	*fullcmd;
  FILE	*stream;

  stream = NULL;
  if ((fullpath = resolve_path(hdl, arg)) != NULL)
    {
      if ((fullcmd = malloc(sizeof(char) *
                            strlen(LS_CMD) +
                            strlen(fullpath) + 2)) == NULL)
        {
          free(fullpath);
          return (NULL);
        }
      sprintf(fullcmd, "%s %s", LS_CMD, fullpath);
      log_msg(DEBUG, "Running command: \"%s\"", fullcmd);
      stream = popen(fullcmd, "r");
    }
  free(fullpath);
  free(fullcmd);
  return (stream);
}
