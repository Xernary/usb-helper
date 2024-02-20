#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "constants.h"

struct name_partition{
  char* name;
  char* partition;
};

struct name_partition* name_partition_map = NULL;
unsigned int map_size = 0;

bool get_partition_from_map(char* name, char* partition){

  if(name_partition_map == NULL){
    perror("name_partition_map is NULL in get_partition_from_map\n");
    exit(1);
  }

  for(int i = 0; i < map_size; i++)
    if(strcmp(name_partition_map[i].name, name) == 0){
      partition = name_partition_map[i].partition;
      return true;
    }
  return false;
}

void add_to_map(char* name, char* partition){
  if(name_partition_map == NULL){
    name_partition_map = malloc(sizeof(struct name_partition) * MAX_MAP_SIZE); 
  }
  if(map_size == MAX_MAP_SIZE) return;
  
  for(int i = 0; i < map_size; i++)
    if(strcmp(name, name_partition_map[i].name) == 0) return;

  struct name_partition new_entry;
  strcpy(new_entry.name, name);
  strcpy(new_entry.partition, partition);
  name_partition_map[map_size] = new_entry;
  map_size++;
}

void show_map(){
  if(name_partition_map == NULL){
    printf("name_partition_map = NULL\n");
    return;
  }
  for(int i = 0; i < map_size; i++){
    printf("name_partition_map[%d] = {%s, %s}\n", i, 
           name_partition_map[i].name, name_partition_map[i].partition);
  }
  printf("\n");
}
