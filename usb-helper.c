#include "heap.h"
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#define MAX_REQUESTS 16
#define MAX_REQUEST_SIZE 5

const char* const path = "/dev/disk/by-path";
const char* const message = "[+] new usb detected";
const char* const substring_1 = "usb";
const char* const substring_2 = "part";
int plugged_usbs = 0;
char** requests = NULL;
unsigned int requests_number = 0;

// letter b/c in sdb/sda is a counter
// so first disk is sda, second sdb and so on

// MAIN DISK name is sda 
// SECOND DISK name sdb is from [sda to sde] and partitions [sda1 to sda9] 
// USB name is sdb

// creates heap with n elements and shows it before and after heapify
void test_heap(int n){
  int initial_array[n];
  srand(time(NULL));
  create_array(n, initial_array);
  show(n, initial_array);

  heapify(n, initial_array);
  show(n, initial_array);
}



int filter(const struct dirent* dirent){
  int select = 0;
  const char* name = dirent->d_name;

  if(strstr(name, substring_1) != NULL && strstr(name, substring_2) != NULL 
     && dirent->d_type == DT_LNK && 
     strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
    select = 1;

  return select;
}



int get_usbs_number(){
  struct dirent** dirents_list;
  int n;
  n = scandir(path, &dirents_list, filter, alphasort);
  for(int i = 0; i < n; i++)
    free(dirents_list[i]);
  free(dirents_list);

  return n;
}



// gets every device file name inside the folder at fd and returns the pointer to its array
int get_names_list(char*** names){
  
  struct dirent** dirents_list;
  int n;
  char** names_list;

  n = scandir(path, &dirents_list, filter, alphasort);
  if(n < 0){
    perror("Error in scandir");
    exit(1);
  }

  names_list = malloc(sizeof(char*) * n);

  for(int i = 0; i < n; i++){
    printf("%s\n", dirents_list[i]->d_name);

    names_list[i] = malloc(sizeof(char) * strlen(dirents_list[i]->d_name) + 1);
    strcpy(names_list[i], dirents_list[i]->d_name); 

    free(dirents_list[i]);
  }
  free(dirents_list);
  
  *names = names_list;

  return n;
}


int new_usbs(){
  return ((get_usbs_number() - plugged_usbs));
}



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

  if(!located && strlen(request) < MAX_REQUEST_SIZE){
    requests[requests_number] = malloc(sizeof(char) * MAX_REQUEST_SIZE);
    strncpy(requests[requests_number++], request, MAX_REQUEST_SIZE);
  }

  return !located;
}


void refactor_array(){
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
      refactor_array();
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


// main deamon loop
void start_detector(){
  int n;
  while(1){
    n = new_usbs();
    if(n > 0){
      printf("%s\n", message);
      // handle the new usb
      plugged_usbs = get_usbs_number();
    }else if(n < 0)
      plugged_usbs = get_usbs_number();
  }
}

int main(){

  int fd;
  char** names_list;  
  //test_heap(10);

  plugged_usbs = get_names_list(&names_list); 

  printf("n: %d\n\n", plugged_usbs);

  //start_detector();

  /*for(int i = 0; i < plugged_usbs; i++){
    printf("%s\n", names_list[i]);
    free(names_list[i]);
  }
  free(names_list);*/

  show_requests();
  add_request("one");
  add_request("two");
  add_request("ccc");
  add_request("aa");
  add_request("bbb");
  add_request("ccc");
  add_request("the");
  show_requests();
  remove_request("two");
  show_requests();
  

  printf("%s", "\nworked\n");
  return 0;
}
