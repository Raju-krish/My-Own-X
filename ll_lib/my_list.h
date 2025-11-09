#define SUCCESS 0
#define FAILURE -1

int insert_list(void *data);
int clear_list();
void for_each_entry(void func(void *data));
