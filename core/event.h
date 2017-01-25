// Event data struture
typedef struct {
  char *name;
  void *next;
  void *previous;
  void (*func)(void *, void *);
  void *udata;
} Event;

// Register an event
Event * event_on(char *name, void *function, void *udata);
void event_trigger(char *name, void *edata);