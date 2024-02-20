#include "heap.h"
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_REQUESTS 16
#define MAX_REQUEST_SIZE 5

const char* const path = "/dev/disk/by-path";
const char* const message = "[+] new usb detected";
const char* const substring_1 = "usb";
const char* const substring_2 = "part";
int plugged_usbs = 0;
char** names_list = NULL;  
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



// gets every device file name inside the folder at path and puts the the pointer to the array of names in names, and returns the number of them
int get_names_list(char*** names){
  
  struct dirent** dirents_list;
  int n;
  char** new_names_list;

  n = scandir(path, &dirents_list, filter, alphasort);
  if(n < 0){
    perror("Error in scandir");
    exit(1);
  }else if(n == 0) return 0;

  new_names_list = malloc(sizeof(char*) * n);

  for(int i = 0; i < n; i++){
    printf("%s\n", dirents_list[i]->d_name);

    new_names_list[i] = malloc(sizeof(char) * strlen(dirents_list[i]->d_name) + 1);
    strcpy(new_names_list[i], dirents_list[i]->d_name); 
    printf("dirents_list[i]->d_name: %s\n", dirents_list[i]->d_name); 

    free(dirents_list[i]);
  }
  free(dirents_list);
  *names = new_names_list;

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


// gets the name of the file that file_path (abs) points to, and copies it in name
int get_link_name(char* file_path, char* name){
  char buffer[1024];
  ssize_t link_string_length;
  if ((link_string_length = readlink(file_path, buffer, sizeof(buffer))) == -1){
    perror(file_path);
  }

  else{
    // Make sure that the buffer is terminated as a string
    buffer[link_string_length] = '\0';
    strcpy(name, buffer);
    printf("%s -> %s\n", file_path, buffer);
  }
}



// gets the usb partition name from its relative path full_name and puts it in partition_name
void get_usb_partition_name(char* full_name, char* partition_name){

  if(full_name == NULL){
    perror("full_name is NULL in get_usb_partition()\n");
    exit(1);
  }
  char tmp[64];
  char absolute_full_name[128];
  /*printf("GET_USB CALLED\n");
  printf("size: %d\n", size);
  printf("full_name: %p\n", (void*) full_name);
  printf("full_name: %s\n", full_name);
  printf("full_name[size-1-3]: %c\n", full_name[size-1-3]);
  printf("&(full_name[0]): %p\n",(void*) &(full_name[0]));
  printf("partition_name: %p\n", (void*) partition_name);*/
  strcpy(absolute_full_name, path);
  strcat(absolute_full_name, "/");
  strcat(absolute_full_name, full_name);
  printf("CONCAT: %s\n", absolute_full_name);
  get_link_name(absolute_full_name, tmp);
  int size = strlen(tmp);
  strcpy(partition_name, &(tmp[size-4]));
  //printf("GET_USB 2 CALLED\n");
}


// compares the new fetched list of usbs to the in-memory one (names_list) and if it finds a new usb that is not in the in-memory, it gets its name and puts it in name
bool find_new_usb(char* name){

  char** all_usbs;
  get_names_list(&all_usbs);
  bool found = false;
  bool is_new = false;

  for(int i = 0; i < get_usbs_number(); i++){
    printf("all: %s\n", all_usbs[i]);
  }


  for(int i = 0; i < get_usbs_number(); i++){
    found = false;
    for(int j = 0; j < plugged_usbs; j++){
      if(strcmp(all_usbs[i], names_list[j]) == 0){ 
        found = true;
        break;
      }
    }
    printf("FIND CALLED\n");
    if(!found || plugged_usbs == 0){
      is_new = true;
      /*printf("name: %p\n", name);
      printf("all_usbs[i]: %p\n", all_usbs[i]);*/
      get_usb_partition_name(all_usbs[i], name);
      break;
    }
  }
  return is_new;
}


// main deamon loop
void start_detector(){
  int n;
  while(1){
    n = new_usbs();
    if(n > 0){
      printf("%s\n", message);
      printf("\n\nNEW USB DETECTED, TOTAL IS: %d\n", n);
      // handle the new usb
      char name[1024];
      char part_name[16]; 


      if(find_new_usb(name)){
        get_usb_partition_name(name, part_name);
        if(add_request(part_name))
          printf("\n+USB: %s, TOTAL: %d\n", name, plugged_usbs+n);
        else{
          printf("\n[/] USB IGNORED, CURRENT REQ_LIST:\n");
          show_requests();
        }
      }
      else
        printf("NOT A NEW USB\n");
      plugged_usbs += n;
      get_names_list(&names_list);
    }else if(n < 0){
      plugged_usbs = get_usbs_number();
      get_names_list(&names_list);
      printf("\n-USB: idk yet, TOTAL: %d\n", plugged_usbs);
    }
  }
}

int main(){

  int fd;
  //test_heap(10);

  plugged_usbs = get_names_list(&names_list); 

  printf("n: %d\n\n", plugged_usbs);

  start_detector();

  /*for(int i = 0; i < plugged_usbs; i++){
    printf("%s\n", names_list[i]);
    free(names_list[i]);
  }
  free(names_list);*/

  char buff[16];
  get_usb_partition_name("pci-0000:00:0b.0-usb-0:1:1.0-scsi-0:0:0:0-part1", buff);
  printf("\nPART NAME: %s\n", buff);

  /*show_requests();
  add_request("one");
  add_request("two");
  add_request("ccc");
  add_request("aa");
  add_request("bbb");
  add_request("ccc");
  add_request("the");
  show_requests();
  remove_request("two");
  show_requests();*/
  

  printf("%s", "\nworked\n");
  return 0;
}
