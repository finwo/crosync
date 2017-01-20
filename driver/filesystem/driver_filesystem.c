#include <stdio.h>

#include "driver.h"

// Prints driver info to stdout
char *driver_filesystem_info() {
  printf("\n");
  printf("Driver: filesystem\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
}

// Returns pointer to CSV list of types
char *driver_filesystem_list_types() {
  
}

// Returns pointer to CSV list of IDs
char *driver_filesystem_list_ids(char *type) {
  
}

// Returns a single object
char *driver_filesystem_get_object(char *type, char *id) {
  
}

// Store an object, returns original if available
char *driver_filesystem_set_object(char *type, char *object) {
  
}

// Delete an object, returns original
char *driver_filesystem_del_object(char *type, char *id) {
  
}

// Returns a CSV list of objects containing the string (key or value)
char *driver_filesystem_objects_containing(char *type, char *search) {
  
}

// Register the driver to main
void register_filesystem_driver (void) __attribute__ ((constructor));
void register_filesystem_driver(void) {
    Driver driver;
    driver.name = "filesystem";
    driver.info       = &driver_filesystem_info;
    driver.types      = &driver_filesystem_list_types;
    driver.ids        = &driver_filesystem_list_ids;
    driver.get        = &driver_filesystem_get_object;
    driver.set        = &driver_filesystem_set_object;
    driver.del        = &driver_filesystem_del_object;
    driver.containing = &driver_filesystem_objects_containing;
    driver_register(&driver);
}