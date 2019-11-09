#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "http.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

void http_header_free(http_header *header) {
  if (header->next) http_header_free(header->next);
  if (header->key) free(header->key);
  if (header->value) free(header->value);
  free(header);
}

char *http_header_get(http_request *request, char *key) {
  http_header *header = request->headers;
  while(header) {
    if (!strcasecmp(key, header->key)) {
      return header->value;
    }
    header = header->next;
  }
  return NULL;
}

void http_request_free(http_request *request) {
  if (request->body) free(request->body);
  if (request->method) free(request->method);
  if (request->path) free(request->path);
  if (request->headers) http_header_free(request->headers);
  free(request);
}

http_request * http_request_init() {
  http_request *request = calloc(1, sizeof(http_request));
  return request;
}

void http_request_data(http_request *request, char *data, int size) {
  http_header *header;
  http_event *ev;
  char *index;
  int newsize;
  char *colon;
  char *buf;
  char *aContentLength;
  int iContentLength;

  // Add event data to buffer
  if (!request->body) request->body = malloc(1);
  request->body = realloc(request->body, request->bodysize + size + 1);
  memcpy(request->body + request->bodysize, data, size);
  request->bodysize += size;

  // Make string functions not segfault
  *(request->body + request->bodysize) = '\0';

  int running = 1;
  while(running) {
    switch(request->state) {
      case HTTP_STATE_PANIC:
        return;
      case HTTP_STATE_METHOD:

        // Wait for more data if not line break found
        index = strstr(request->body, "\r\n");
        if (!index) return;
        *(index) = '\0';

        // Read method and path
        request->method = calloc(1, 7);
        request->path   = calloc(1, 512);
        if (sscanf(request->body, "%6s %511s", request->method, request->path) != 2) {
          request->state = HTTP_STATE_PANIC;
          return;
        }

        // Remove the method line
        newsize = request->bodysize - 2 - (index - request->body);
        buf     = calloc(1,newsize+1);
        memcpy(buf, index + 2, newsize);
        free(request->body);
        request->body = buf;
        request->bodysize = newsize;

        // Signal we're now reading headers
        request->state = HTTP_STATE_HEADER;
        break;

      case HTTP_STATE_HEADER:

        // Wait for more data if not line break found
        index = strstr(request->body, "\r\n");
        if (!index) return;
        *(index) = '\0';

        // Detect end of headers
        newsize = strlen(request->body);
        if (!newsize) {

          // Remove the blank line
          newsize = request->bodysize - 2;
          buf = calloc(1,newsize+1);
          memcpy(buf, index + 2, newsize);
          free(request->body);
          request->body = buf;
          request->bodysize = newsize;

          // GET/DELETE = start responding
          if (
              (!strcmp(request->method, "GET")) ||
              (!strcmp(request->method, "DELETE"))
          ) {
            request->state = HTTP_STATE_RESPONSE;
            break;
          }

          request->state = HTTP_STATE_BODY;
          break;
        }

        // Prepare new header
        header = calloc(1,sizeof(header));
        header->key = calloc(1,strlen(request->body));
        header->value = calloc(1,strlen(request->body));

        // Copy key & value
        colon = strstr(request->body, ":");
        if (colon) {
          *(colon) = '\0';
          strcpy(header->key, request->body);
          strcpy(header->value, colon + 1);
        }

        // Assign to the header list
        header->next = request->headers;
        request->headers = header;

        // Remove the header line
        newsize = request->bodysize - 2 - (index - request->body);
        buf = calloc(1,newsize+1);
        memcpy(buf, index + 2, newsize);
        free(request->body);
        request->body = buf;
        request->bodysize = newsize;

        break;

      case HTTP_STATE_BODY:

        // Fetch the content length
        aContentLength = http_header_get(request, "content-length");
        iContentLength = atoi(aContentLength);

        // Not enough data = skip
        if (request->bodysize < iContentLength) {
          running = 0;
          break;
        }

        // Change size to indicated size
        request->bodysize = iContentLength;
        request->state = HTTP_STATE_RESPONSE;
        break;

      case HTTP_STATE_RESPONSE:

        if (request->onRequest) {
          ev = calloc(1,sizeof(http_event));
          ev->request = request;
          request->onRequest(ev);
          request->onRequest = NULL;
        }

        running = 0;
        break;
    }
  }
}

#ifdef __cplusplus
} // extern "C"
#endif
