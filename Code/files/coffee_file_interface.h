typedef struct {
int error;
Str63 name;
} coffee_file_spec;	// if changed please change root.zsl

//errors
#define NOERR 0	
#define FOLDERERR -1	//W folder missing
#define INDEXERR -2	//index error (runs 1 to max) - file not found

#define OPENERR -3
#define EOFERR -4
#define READERR -5
#define CLOSEERR -6
#define WRITEERR -7
#define CREATERR -8


//stu test routine 
void cfi_test(void);

extern unsigned char *directory;		// removed const - we pass to an OS file routine that doesn't have a const parameter

//given an index return a pointer to a coffee_file_spec that gives the name of the file
coffee_file_spec* coffee_get_file_spec(int index);
//given a file name returns the size of it or -error
int coffee_get_file_size(char* name);
//given a file name and data to space reads a file
int coffee_read_file(char* name, char* data_goes_here);
//write a file to the W folder
int coffee_write_file(char* name, char* data_to_write, int length);

void pstrcopy(char* from, char* to);