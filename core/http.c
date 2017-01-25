#include <stdio.h>
#include <stdlib.h>

#include "dyad.h"
#include "http.h"

dyad_Stream *serv;

static void http_on_close(dyad_Event *e)
{
  Client *client = e->udata;
  free(client);
}

static void http_on_line(dyad_Event *e)
{
  dyad_end(e->stream);
}

static void http_on_accept(dyad_Event *e)
{
  Client *client = calloc(1, sizeof(Client));
  printf("Client accepted\n");
  dyad_addListener(e->remote, DYAD_EVENT_LINE,  http_on_line,  client);
  dyad_addListener(e->remote, DYAD_EVENT_CLOSE, http_on_close, client);
}

static void http_on_listen(dyad_Event *e)
{
  printf("Server listening on port %d\n", dyad_getPort(e->stream));
}

static void http_on_error(dyad_Event *e)
{
  fprintf(stderr, "Error: %s\n", e->msg);
}

void http_start(int port)
{
  serv = dyad_newStream();
  dyad_addListener(serv, DYAD_EVENT_ERROR,  http_on_error,  NULL);
  dyad_addListener(serv, DYAD_EVENT_ACCEPT, http_on_accept, NULL);
  dyad_addListener(serv, DYAD_EVENT_LISTEN, http_on_listen, NULL);
  dyad_listen(serv, port);
}

int http_poll()
{
  if(dyad_getStreamCount()>0) {
    dyad_update();
    return 1;
  } else {
    dyad_shutdown();
    return 0;
  }
}