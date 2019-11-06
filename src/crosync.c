#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dyad.h"

static int count = 0;

static void onLine(dyad_Event *e) {
  char path[128];
  char method[7];

  // No method || path = kill
  if (sscanf(e->data, "%6s %127s", method, path) != 2) {
    dyad_writef(e->stream,
      "HTTP/1.1 400 Bad Request\n\n"
    );
    dyad_writef(e->stream,
      "<!DOCTYPE html>"
      "<html>"
        "<body>"
          "Invalid HTTP request"
        "</body>"
      "</html>"
    );
    dyad_end(e->stream);
    return;
  }

  dyad_writef(e->stream,
    "HTTP/1.1 200 OK\n\n"
  );
  dyad_writef(e->stream,
    "METHOD %s\n",
    method
  );
  dyad_writef(e->stream,
    "PATH   %s\n",
    path
  );

  dyad_end(e->stream);
}

static void onAccept(dyad_Event *e) {
  dyad_addListener(e->remote, DYAD_EVENT_LINE, onLine, NULL);
}

static void onListen(dyad_Event *e) {
  printf("server listening: http://localhost:%d\n", dyad_getPort(e->stream));
}

static void onError(dyad_Event *e) {
  printf("server error: %s\n", e->msg);
}

int main(int argc, char *argv[]) {
  int port = 8080;

  int opt = 0;
  static struct option long_options[] = {
    { "port", required_argument, 0, 'p' },
    { 0,      0,                 0, 0   },
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
