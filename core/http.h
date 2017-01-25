#include "dyad.h"

typedef struct {
  dyad_Stream *stream;
} Client;

void http_start(int port);
int http_poll();