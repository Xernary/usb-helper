#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"

char** requests = NULL;
unsigned int requests_number = 0;

// checks if partition_name is mounted on the system
bool is_req_mounted(char* partition_name){
  bool mounted = false;

  // open the system mount-point folder and search for folder named partiition_name
  // return true if found, false otherwise

  return mounted;
}

// add req if not present already and request its not already mounted
bool add_request(char* request){
  bool inserted = false;

  if(requests == NULL){
    // allocate requests array
    requests = malloc(sizeof(char*) * MAX_REQUESTS);
  }

  bool located = false;
  for(int i = 0; i < requests_number; i++)
    if(strcmp(requests[i], request) == 0){
      located = true;
      break;
    }
                                                       // to do
  if(!located && strlen(request) < MAX_REQUEST_SIZE && !is_req_mounted(request)){
    requests[requests_number] = malloc(sizeof(char) * MAX_REQUEST_SIZE);
    strncpy(requests[requests_number++], request, MAX_REQUEST_SIZE);
  }

  return !located;
}

void refactor_req_array(){
  for(int i = 0; i < requests_number-1; i++){
    for(int j = i+1; j < requests_number; j++){
      if(requests[i] != NULL) break;
      if(requests[j] != NULL){
        requests[i] = requests[j];
        requests[j] = NULL;
      }
    }
  }
}

bool remove_request(char* request){
  bool deleted = false;

  if(requests == NULL) return false;

  for(int i = 0; i < requests_number; i++){
    if(strcmp(requests[i], request) == 0){
      free(requests[i]);
      requests[i] = NULL;
      refactor_req_array();
      requests_number--;
      deleted = true;
    }
  }
  return deleted;
}

void show_requests(){
  if(requests_number == 0){
    printf("requests array is empty\n");
  }
  else{
    for(int i = 0; i < requests_number; i++)
      printf("request[%d]: %s\n", i, requests[i]);
  }
  printf("\n");
}
