#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dyad.h"
#include "http-parser.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define CROSYNC_INIT 0
#define CROSYNC_HEAD 1
#define CROSYNC_BODY 2
#define CROSYNC_RESP 3

static void onRequest(struct http_parser_event *ev) {
  dyad_Event *e = ev->request->udata;
  struct http_parser_request *request = ev->request;
  dyad_writef(e->stream,
      "HTTP/1.0 200 OK\r\n"
      "\r\n"
      "Method: %s\r\n"
  , request->method);
  dyad_end(e->stream);
}

static void onData(dyad_Event *e) {
  struct http_parser_request *request = e->udata;
  request->udata = e;
  http_parser_request_data(request, e->data, e->size);
}

static void onAccept(dyad_Event *e) {
  struct http_parser_request *request = http_parser_request_init();
  request->onRequest = onRequest;
  dyad_addListener(e->remote, DYAD_EVENT_DATA, onData, request);
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

#ifdef __cplusplus
} // extern "C"
#endif
