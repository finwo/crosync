// How the drivers should be registered
typedef struct {
  char *name;
  void *info;
  void *types;
  void *ids;
  void *get;
  void *set;
  void *del;
  void *containing;
} Driver;

// Register a new driver
void driver_register( Driver *driver );