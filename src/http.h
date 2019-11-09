#ifndef CROSYNC_HTTP_H
#define CROSYNC_HTTP_H

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_STATE_METHOD   0
#define HTTP_STATE_HEADER   1
#define HTTP_STATE_BODY     2
#define HTTP_STATE_RESPONSE 3
#define HTTP_STATE_PANIC    666

typedef struct {
  void *next;
  char *key;
  char *value;
} http_header;

typedef struct {
  struct http_request *request;
} http_event;

typedef void (*http_callback)(http_event*);

typedef struct http_request {
  int bodysize;
  int state;
  char *method;
  char *path;
  http_header *headers;
  char *body;
  http_callback onRequest;
  void *udata;
} http_request;



char *http_header_get(http_request *request, char *key);
void http_request_free(http_request *request);
http_request * http_request_init();
void http_request_data(http_request *request, char *data, int size);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CROSYNC_HTTP_H
