//Error_handler.ext
void report_error(const char *error_string, const Str255 error_string2, int error_number);

// not implemented yet for Zex2
void report_error_c(const char *error_string, const char * error_string2, int error_number);

void report_error_id(const char *error_String,unsigned long id);
void report_error_id_number(const char *error_string, unsigned long id, int number);
void report_error_id_non_fatal(const char *error_string, unsigned long id);


void MyDebugStr( short line, const char* file, const char* text);

