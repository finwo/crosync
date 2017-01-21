// How the drivers should be registered
typedef struct {
  char *name;
  void *info;
  void *init;
  void *types;
  void *ids;
  void *get;
  void *set;
  void *del;
  void *containing;
} Driver;

// Register a new driver
void driver_register( Driver *driver );

// The actual driver functions to be called
char * driver_info();                                       // Print driver info to stdout
char * driver_init(char *arg);                              // Let the driver know it's gonna be used
char * driver_types();                                      // Returns pointer to CSV list of types
char * driver_ids(char *type);                              // Returns pointer to CSV list of IDs
char * driver_get_object(char *type, char *id);             // Returns a single object
char * driver_set_object(char *type, char *object);         // Store an object, returns original if available
char * driver_del_object(char *type, char *id);             // Delete an object, returns original
char * driver_objects_containing(char *type, char *search); // Returns a CSV list of objects containing the string (key or value)

// List all drivers to stdout
void driver_list();

// Returns whether the driver (by name) exists
int driver_select(char *name);