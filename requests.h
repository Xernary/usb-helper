#ifndef REQUESTS_C
#define REQUESTS_C

extern char** requests;
extern unsigned int requests_number;

bool is_mounted(char*);

bool add_request(char*);

void refactor_array();

bool remove_request(char*);

void show_requests();

#endif
