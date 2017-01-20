#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver.h"

Driver *drivers;
Driver *selectedDriver;
int drivers_count = 0;

int driverPointerSize = sizeof(Driver);

// Register a new driver
void driver_register( Driver *driver )
{
  // Increment counter
  drivers_count++;
  
  // The list may move
  char *newListLocation;
  
  // Make sure we have the memory
  if ( drivers ) {
    newListLocation = realloc( drivers, driverPointerSize * drivers_count );
    if ( newListLocation ) {
      drivers = (void *)newListLocation;
    } else {
      exit(1);
    }
  } else {
    drivers = malloc( driverPointerSize * drivers_count );
  }
  
  // 'Register' the listener
  *(drivers+((drivers_count-1)*driverPointerSize)) = *driver;
}


// List all drivers to stdout
void driver_list() {
  int i;
  Driver *driver;
  for( i = 0 ; i < drivers_count ; i++ ) {
    driver = drivers + ( driverPointerSize * i );
    printf("  %s\n", driver->name);
  }
}

// Select a driver
// Returns if we were successful
int driver_select(char *name) {
  int i;
  Driver *driver;
  for( i = 0 ; i < drivers_count ; i++ ) {
    driver = drivers + ( driverPointerSize * i );
    if(!strcmp(driver->name, name)) {
      selectedDriver = driver;
      return 1;
    }
  }
  return 0;
}

// Structures for calling the driver's functions
char * (*drv0)();
char * (*drv1)(char *a0);
char * (*drv2)(char *a0, char *a1);

char * driver_info() {
  drv0 = selectedDriver->info;
  return (*drv0)();
}
char * driver_types() {
  drv0 = selectedDriver->types;
  return (*drv0)();
}
char * driver_ids(char *type) {
  drv1 = selectedDriver->ids;
  return (*drv1)(type);
}
char * driver_get_object(char *type, char *id) {
  drv2 = selectedDriver->get;
  return (*drv2)(type,id);
}
char * driver_set_object(char *type, char *object) {
  drv2 = selectedDriver->set;
  return (*drv2)(type,object);
}
char * driver_del_object(char *type, char *id) {
  drv2 = selectedDriver->get;
  return (*drv2)(type,id);
}
char * driver_objects_containing(char *type, char *search) {
  drv2 = selectedDriver->get;
  return (*drv2)(type,search);
}