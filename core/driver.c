#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver.h"

void **driver_list      = 0;
int driver_list_length  = 0;
int driver_pointer_size = sizeof(Driver *);
Driver *driver;

void drivers_list()
{
  Driver *current_driver;
  int i;
  for(i=0;i<driver_list_length;i++) {
    current_driver = *(driver_list + (driver_pointer_size*i));
    printf("  %s\n", current_driver->name);
  }
}

void driver_register( Driver *new_driver )
{
  driver_list_length++;
  void *newListLocation = 0;
  if(driver_list) {
    newListLocation = realloc(driver_list, driver_pointer_size * driver_list_length);
    if(newListLocation) {
      driver_list = newListLocation;
    } else {
      exit(3);
    }
  } else {
    driver_list = calloc(driver_list_length, driver_pointer_size);
  }
  *(driver_list + ((driver_list_length-1)*driver_pointer_size) ) = new_driver;
}

int driver_select(char *name)
{
  int i;
  for(i=0;i<driver_list_length;i++) {
    driver = *(driver_list + ( i * driver_pointer_size ));
    if(strcmp(driver->name, name)==0) {
      return 1;
    }
  }
  return 0;
}

void driver_info()
{
  (*driver->info)();
}