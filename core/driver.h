
typedef struct {
  char *name;
  void (*info)();
} Driver;

void drivers_list();
void driver_register( Driver *driver );
int driver_select(char *name);
void driver_info();