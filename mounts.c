#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"

char** mounts = NULL;
unsigned int mounts_number = 0;

// checks if partition_name is mounted on the system
bool is_mounted(char* partition_name){
  bool mounted = false;

  // open the system mount-point folder and search for folder named partiition_name
  // return true if found, false otherwise

  return mounted;
}

// add mount if not present already and mount ifs not already mounted
bool add_mount(char* mount){
  bool inserted = false;

  if(mounts == NULL){
    // allocate mounts array
    mounts = malloc(sizeof(char*) * MAX_REQUESTS);
  }

  bool located = false;
  for(int i = 0; i < mounts_number; i++)
    if(strcmp(mounts[i], mount) == 0){
      located = true;
      break;
    }
                                                       // to do
  if(!located && strlen(mount) < MAX_REQUEST_SIZE && !is_mounted(mount)){
    mounts[mounts_number] = malloc(sizeof(char) * MAX_REQUEST_SIZE);
    strncpy(mounts[mounts_number++], mount, MAX_REQUEST_SIZE);
  }

  return !located;
}

void refactor_array(){
  for(int i = 0; i < mounts_number-1; i++){
    for(int j = i+1; j < mounts_number; j++){
      if(mounts[i] != NULL) break;
      if(mounts[j] != NULL){
        mounts[i] = mounts[j];
        mounts[j] = NULL;
      }
    }
  }
}

bool remove_mount(char* mount){
  bool deleted = false;

  if(mounts == NULL) return false;

  for(int i = 0; i < mounts_number; i++){
    if(strcmp(mounts[i], mount) == 0){
      free(mounts[i]);
      mounts[i] = NULL;
      refactor_array();
      mounts_number--;
      deleted = true;
    }
  }
  return deleted;
}

void show_mounts(){
  if(mounts_number == 0){
    printf("mounts array is empty\n");
  }
  else{
    for(int i = 0; i < mounts_number; i++)
      printf("mount[%d]: %s\n", i, mounts[i]);
  }
  printf("\n");
}
