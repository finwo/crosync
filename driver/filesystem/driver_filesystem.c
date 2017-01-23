#include <stdio.h>
#include <stdlib.h>

#include "driver.h"

// Register the driver to main
void register_filesystem_driver (void) __attribute__ ((constructor));
void register_filesystem_driver(void) {
    Driver *driver = malloc(sizeof(Driver));
    driver->name = "filesystem";
    driver_register(driver);
}