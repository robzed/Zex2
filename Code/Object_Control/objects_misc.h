//objects_misc.ext

int find_dynamic_object_slot(unsigned long the_id);
int point_in_rect(int x, int y, LSRect the_rect);
int point_in_float_rect(float x, float y, LSFloatRect the_rect);

void calculate_object_near_flag(void);
int check_for_object_near(void);
