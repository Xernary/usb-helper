#ifndef REQUESTS_C
#define REQUESTS_C

extern char** requests;
extern unsigned int requests_number;

bool add_request(char*);

bool remove_request(char*);

void show_requests();

#endif
