void get_coffee(void);
void run_coffee(void);
void coffee_start_new_game(void);
void coffee_end_game(void);
void coffee_object_created(int id);
void coffee_object_deleted(int id);

void complete_coffee_setup(void);

int interpret(char *string);
void declare_variable(char *name, void *var_name);
void declare_constant(char *name, int const_name);

#define declare_function(name,function,parameters,return_param) coffee5(declare_c_function,(long)name,(long)function,parameters,return_param)
#define declare_advanced_function(name,function,parameters,return_param) coffee5(declare_func_advanced,(long)name,(long)function,parameters,return_param)
