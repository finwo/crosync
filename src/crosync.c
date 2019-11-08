#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dyad.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define CROSYNC_INIT 0
#define CROSYNC_HEAD 1
#define CROSYNC_BODY 2
#define CROSYNC_RESP 3

char *strlwr(char *str) {
  unsigned char *p = (unsigned char *)str;
  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }
  return str;
}

typedef struct {
  void *next;
  char *key;
  char *value;
} http_header;

typedef struct {
  char *raw;
  int rawsize;
  int state;
  char *method;
  char *path;
  http_header *headers;
  void *body;
  int bodysize;
} http_request;

void http_header_free(http_header *header) {
  if (header->next) http_header_free(header->next);
  if (header->key) free(header->key);
  if (header->value) free(header->value);
  free(header);
}

char *http_header_get(http_header *header, char *key) {
  while(header) {
    if (!strcasecmp(key, header->key)) {
      return header->value;
    }
    header = header->next;
  }
  return NULL;
}

void http_request_free(http_request *request) {
  if (request->raw) free(request->raw);
  if (request->method) free(request->method);
  if (request->path) free(request->path);
  if (request->headers) http_header_free(request->headers);
  if (request->body) free(request->body);
  free(request);
}

// Copies data from the net into the holding buffer
static void onData(dyad_Event *e) {
  http_request *request = e->udata;
  http_header *header;
  char *index;
  char *colon;
  char *buf;
  int size;
  char *aContentLength;
  int iContentLength;

  // Add event data to buffer
  if (!request->raw) request->raw = malloc(1);
  request->raw = realloc(request->raw, request->rawsize + e->size + 1);
  memcpy(request->raw + request->rawsize, e->data, e->size);
  request->rawsize += e->size;
  *(request->raw + request->rawsize) = '\0';

  int running = 1;
  while(running) {
    switch(request->state) {
      case CROSYNC_INIT:

        // Wait for more data if not line break found
        index = strstr(request->raw, "\r\n");
        if (!index) return;
        *(index) = '\0';

        // Read method and path
        request->method = calloc(1, 7);
        request->path   = calloc(1, 128);
        if (sscanf(request->raw, "%6s %127s", request->method, request->path) != 2) {
          http_request_free(request);
          dyad_end(e->stream);
          return;
        }

        // Remove the method line
        size = request->rawsize - 2 - (index - request->raw);
        buf = calloc(1,size+1);
        memcpy(buf, index + 2, size);
        free(request->raw);
        request->raw = buf;
        request->rawsize = size;

        printf("METHOD: %s\n", request->method);

        // Signal we're now reading headers
        request->state = CROSYNC_HEAD;
        break;

      case CROSYNC_HEAD:

        // Wait for more data if not line break found
        index = strstr(request->raw, "\r\n");
        if (!index) return;
        *(index) = '\0';

        // Detect end of headers
        size = strlen(request->raw);
        if (!size) {

          // Remove the blank line
          size = request->rawsize - 2;
          buf = calloc(1,size+1);
          memcpy(buf, index + 2, size);
          free(request->raw);
          request->raw = buf;
          request->rawsize = size;

          // GET/DELETE = start responding
          if (!strcmp(request->method, "GET")) {
            request->state = CROSYNC_RESP;
            running = 0;
            break;
          }
          if (!strcmp(request->method, "DELETE")) {
            request->state = CROSYNC_RESP;
            running = 0;
            break;
          }

          request->state = CROSYNC_BODY;
          break;
        }

        // Prepare new header
        header = calloc(1,sizeof(header));
        header->key = calloc(1,strlen(request->raw));
        header->value = calloc(1,strlen(request->raw));

        // Copy key & value
        colon = strstr(request->raw, ":");
        if (colon) {
          *(colon) = '\0';
          strcpy(header->key, request->raw);
          strcpy(header->value, colon + 1);
        }

        // Assign to the header list
        header->next = request->headers;
        request->headers = header;

        // Remove the header line
        size = request->rawsize - 2 - (index - request->raw);
        buf = calloc(1,size+1);
        memcpy(buf, index + 2, size);
        free(request->raw);
        request->raw = buf;
        request->rawsize = size;

        break;

      case CROSYNC_BODY:

          aContentLength = http_header_get(request->headers, "content-length");
          iContentLength = atoi(aContentLength);
          printf("Content length: %d\n", iContentLength);

          // Indicate we're starting to read the body

        break;
    }
  }
}

static void onAccept(dyad_Event *e) {
  http_request *request = calloc(1, sizeof(http_request));
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
