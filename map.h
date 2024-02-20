#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

extern struct name_partition* name_partition_map;
extern unsigned int map_size;

bool get_partition_from_map(char*, char*);

void add_to_map(char*, char*);

void show_map();

#endif
