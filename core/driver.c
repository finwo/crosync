#include <stdio.h>
#include <stdlib.h>

#include "driver.h"

void **driver_list      = 0;
int driver_list_length  = 0;
int driver_pointer_size = sizeof(Driver *);

void drivers_list()
{
  Driver *current_driver;
  int i;
  for(i=0;i<driver_list_length;i++) {
    current_driver = *(driver_list + (driver_pointer_size*i));
    printf("  %s\n", current_driver->name);
  }
}

void driver_register( Driver *driver )
{
  driver_list_length++;
  void *newListLocation = 0;
  if(driver_list) {
    newListLocation = realloc(driver_list, driver_pointer_size * driver_list_length);
    if(newListLocation) {
      driver_list = newListLocation;
    } else {
      exit(1);
    }
  } else {
    driver_list = malloc(driver_pointer_size*driver_list_length);
  }
  *(driver_list + ((driver_list_length-1)*driver_pointer_size) ) = driver;
}