#ifndef MOUNTS_C
#define MOUNTS_C

extern char** mounts;
extern unsigned int mounts_number;

bool add_mount(char*);

bool remove_mount(char*);

void show_mounts();

#endif
