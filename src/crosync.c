#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dyad.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef struct {
  void *next;
  void *key;
  void *value;
} http_header;

typedef struct {
  char *method;
  char *path;
  http_header *headers;
} http_request;

static void free_header(http_header *header) {
  if (header->next) free_header(header->next);
  if (header->key) free(header->key);
  if (header->value) free(header->value);
  free(header->value);
}

static void free_request(http_request *request) {
  if (request->method) free(request->method);
  if (request->path) free(request->path);
  if (request->headers) free_header(request->headers);

  free(request);
}

static void badRequest(dyad_Event *e) {
  dyad_writef(e->stream,
    "HTTP/1.1 400 Bad Request\n"
    "Content-Type: text/plain\n"
    "\n"
    "Bad Request"
  );
}

static void onLine(dyad_Event *e) {
  http_request *request = e->udata;

  // No method = first line
  if (!request->method) {
    request->method = calloc(1, 7);
    request->path = calloc(1, 128);

    // No method||path = kill
    if (sscanf(e->data, "%6s %127s", request->method, request->path) != 2) {
      free_request(request);
      badRequest(e);
      return;
    }

    // Done for this line
    return;
  }

  // !strlen(line) -> get = done

  // Headers here
  // newheader->next = req->headers
  // req->headers = newheader
  // newheader[key,value] = line.split(:)

  printf("LINE: %s\n", e->data);
  dyad_end(e->stream);
  return;

  /* dyad_writef(e->stream, */
  /*   "HTTP/1.1 200 OK\n\n" */
  /* ); */
  /* dyad_writef(e->stream, */
  /*   "METHOD %s\n", */
  /*   method */
  /* ); */
  /* dyad_writef(e->stream, */
  /*   "PATH   %s\n", */
  /*   path */
  /* ); */

  /* dyad_end(e->stream); */
}

static void onAccept(dyad_Event *e) {
  http_request *request = calloc(1, sizeof(http_request));
  dyad_addListener(e->remote, DYAD_EVENT_LINE, onLine, request);
}

static void onListen(dyad_Event *e) {
  printf("server listening: http://localhost:%d\n", dyad_getPort(e->stream));
}

static void onError(dyad_Event *e) {
  printf("server error: %s\n", e->msg);
}

int main(int argc, char *argv[]) {
  int port        = 8080;
  int interactive = 1;

  int opt = 0;
  static struct option long_options[] = {
    { "port"  , required_argument, 0, 'p' },
    { 0       , 0                , 0, 0   },
  };

  int long_index = 0;
  while ((opt = getopt_long(argc, argv, "p:",
          long_options, &long_index )) != -1) {
    switch(opt) {
      case 'p':
        port = atoi(optarg);
        break;
    }
  }

  dyad_Stream *s;
  dyad_init();

  s = dyad_newStream();
  dyad_addListener(s, DYAD_EVENT_ERROR,  onError,  NULL);
  dyad_addListener(s, DYAD_EVENT_ACCEPT, onAccept, NULL);
  dyad_addListener(s, DYAD_EVENT_LISTEN, onListen, NULL);
  dyad_listen(s, port);

  while (dyad_getStreamCount() > 0) {
    dyad_update();
  }

  dyad_shutdown();
  return 0;
}
