#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver.h"
#include "driver_usage.h"

Driver *drivers;
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

int driver_select(char *name) {
  int i;
  Driver *driver;
  for( i = 0 ; i < drivers_count ; i++ ) {
    driver = drivers + ( driverPointerSize * i );
    if(!strcmp(driver->name, name)) {
      
      // Copy pointers from the selected driver
      driver_info               = driver->info;
      driver_types              = driver->types;
      driver_ids                = driver->ids;
      driver_get_object         = driver->get;
      driver_set_object         = driver->set;
      driver_del_object         = driver->del;
      driver_objects_containing = driver->containing;
      
      return 1;
    }
  }
  return 0;
}