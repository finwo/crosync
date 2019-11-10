#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
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

typedef struct {
  void *next;
  time_t iat;
  char *key;
  char *value;
  int size;
} kv;

kv *data = NULL;

static void list(struct http_parser_request *request) {
  dyad_Event *e = request->udata;
  kv *entry = data;
  printf("LIST\n");

  char *now = calloc(1, 32);
  time_t now_t = time(NULL);
  struct tm* now_to = gmtime(&now_t);
  strftime(now, 32, "%Y-%m-%dT%H:%m:%SZ", now_to);

  if (!entry) {
    dyad_writef(e->stream,
      "HTTP/1.0 204 No Content\r\n"
      "Date: %s\r\n"
      "\r\n"
    , now);
    free(now);
    return;
  }

  // Header start
  dyad_writef(e->stream,
    "HTTP/1.0 200 OK\r\n"
    "Date: %s\r\n"
    "Transfer-Encoding: chunked\r\n"
    "\r\n"
  , now);
  free(now);

  // Write our data
  while(entry) {
    dyad_writef(e->stream, "%x\r\n"    , strlen(entry->key) + 2);
    dyad_writef(e->stream, "%s\r\n\r\n",        entry->key     );
    entry = entry->next;
  }

  dyad_write(e->stream, "0\r\n\r\n", 5);
}

static void read(struct http_parser_request *request, char *key) {
  dyad_Event *e = request->udata;
  kv *entry = data;

  char *now = calloc(1, 32);
  time_t now_t = time(NULL);
  struct tm* now_to = gmtime(&now_t);
  strftime(now, 32, "%Y-%m-%dT%H:%m:%SZ", now_to);

  // Showing the list
  if (!strcmp("/", request->path)) {
    list(request);
    free(now);
    return;
  }

  while(entry) {
    if (!strcmp(key, entry->key)) {
      dyad_writef(e->stream,
        "HTTP/1.0 200 OK\r\n"
        "Date: %s\r\n"
      , now);
      free(now);
      dyad_writef(e->stream,
        "Content-Length: %d\r\n"
        "\r\n"
      , entry->size);
      dyad_write(e->stream, entry->value, entry->size);
      return;
    }
    entry = entry->next;
  }


  dyad_writef(e->stream,
      "HTTP/1.0 404 Not Found\r\n"
      "Date: %s\r\n"
      "\r\n"
      "Not Found\r\n"
  , now);
  free(now);
}

static void write(struct http_parser_request *request, char *key) {
  dyad_Event *e = request->udata;
  kv *entry;

  char *now = calloc(1, 32);
  time_t now_t = time(NULL);
  struct tm* now_to = gmtime(&now_t);
  strftime(now, 32, "%Y-%m-%dT%H:%m:%SZ", now_to);

  // No key/value = error
  if ((!strcmp("/", request->path)) || (!request->body)) {
    dyad_writef(e->stream,
      "HTTP/1.0 400 Bad Request\r\n"
      "Date: %s\r\n"
      "\r\n"
    , now);
    free(now);
    return;
  }

  // Push into stack
  entry = malloc(sizeof(kv));
  entry->size = request->bodysize;
  entry->next = data;
  entry->iat = time(NULL);
  entry->key = calloc(1,strlen(key)+1);
  entry->value = calloc(1,strlen(request->body)+1);
  strcpy(entry->key, key);
  strcpy(entry->value, request->body);
  data = entry;

  // No return value
  dyad_writef(e->stream,
    "HTTP/1.0 204 No Content\r\n"
    "Date: %s\r\n"
    "\r\n"
  , now);
  free(now);
}

static void delete(struct http_parser_request *request, char *key) {
  dyad_Event *e = request->udata;
  dyad_writef(e->stream,
      "HTTP/1.0 200 OK\r\n"
      "\r\n"
      "DELETE %s\r\n"
  , key);
}

static void onRequest(struct http_parser_event *ev) {
  struct http_parser_request *request = ev->request;
  dyad_Event *e = request->udata;

  // Fetch the key
  char *key = calloc(1,8192);
  sscanf(request->path, "/%s", key);

  // We're reading
  if (!strcasecmp(request->method, "GET")) {
    read(request, key);
  }

  // We're writing
  if (!strcasecmp(request->method, "POST")) {
    write(request, key);
  }

  // We're deleting
  if (!strcasecmp(request->method, "DELETE")) {
    delete(request, key);
  }

  free(key);
  dyad_end(e->stream);
  http_parser_request_free(request);
  return;
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
  char *aPort = getenv("PORT");
  int   iPort = 5000;
  int interactive = 1;

  if (aPort) {
    iPort = atoi(aPort);
  }

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
        iPort = atoi(optarg);
        break;
    }
  }

  dyad_Stream *s;
  dyad_init();

  s = dyad_newStream();
  dyad_addListener(s, DYAD_EVENT_ERROR,  onError,  NULL);
  dyad_addListener(s, DYAD_EVENT_ACCEPT, onAccept, NULL);
  dyad_addListener(s, DYAD_EVENT_LISTEN, onListen, NULL);
  dyad_listen(s, iPort);

  while (dyad_getStreamCount() > 0) {
    dyad_update();
  }

  dyad_shutdown();
  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif
