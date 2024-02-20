#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>




// constants.h

#include "constants.h"

// end constants.h



// map.h

#include "map.h"

// end map.h






// requests.h

#include "requests.h"

// end requests.h









int plugged_usbs = 0;
char** names_list = NULL;  

// letter b/c in sdb/sda is a counter
// so first disk is sda, second sdb and so on

// names_list = [pci-0000:00:0b.0-usb-0:1:1.0-scsi-0:0:0:0-part1, ...]
// its the list of unique usbs paths loaded in memory (contains "usb" and "part")
// two unique names can point to the same usb (to the same partition)
// the list gets updated only after a NEW adequate name is found in path
// a new string gets inserted only when its not already in requests[] and its not mounted
// 
// plugged_usbs = 0 
// its the lenght of names_list
//
// requests = ["sdb1", "sdc1", "sdd1", ...]
// an array of string with the NEW detected usb that needs to be mounted (or not)
// a NEW detected usb can only be added in request[] if not already present and if not already mounted
// the requests gets elaborated by asking the user if he wants to mount the usb inside requests[] or not, after this they get removed from requests[]





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
int get_names_list(const char* const file_path, char*** names){
  
  struct dirent** dirents_list;
  int n;
  char** new_names_list;

  n = scandir(file_path, &dirents_list, filter, alphasort);
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


void show_names_list(){

  printf("\n");
  if(plugged_usbs == 0){
    printf("names_list = NULL\n");
    return;
  }

  for(int i = 0; i < plugged_usbs; i++)
    printf("names_list[%d] = %s\n", i, names_list[i]);
  printf("\n");

}


int new_usbs(){
  return ((get_usbs_number() - plugged_usbs));
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
bool find_new_usb(char* full_name, char* name){

  char** all_usbs;
  get_names_list(path, &all_usbs);
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
      strcpy(full_name, all_usbs[i]);
      break;
    }
  }
  return is_new;
}



bool find_unplugged_usb(char* full_name, char* name){

  char** all_usbs;
  get_names_list(path, &all_usbs);
  bool found = false;
  bool is_new = false;

  for(int i = 0; i < get_usbs_number(); i++){
    printf("all: %s\n", all_usbs[i]);
  }


  for(int i = 0; i < plugged_usbs; i++){
    found = false;
    for(int j = 0; j < get_usbs_number(); j++){
      if(strcmp(all_usbs[j], names_list[i]) == 0){ 
        found = true;
        break;
      }
    }
    printf("FIND CALLED\n");
    if(!found && plugged_usbs != 0){
      is_new = true;
      printf("NAME: %s\n", names_list[i]); 
      if(get_partition_from_map(names_list[i], name)){
        strcpy(full_name, names_list[i]);
        printf("NAMEEEEEEEEE: %s\n", name);
      }
      break;
    }
  }
  return is_new;
}




void show_info(){
  printf("------------------------\n");
  printf("plugged_usbs = %d\n\n", plugged_usbs);
  printf("map_size = %d\n\n", map_size);
  show_requests();
  show_names_list();
  show_map();
  printf("------------------------\n");
}


// main deamon loop
void start_detector(){
  int n;
  char part_name[16]; 
  char full_name[64];

  while(1){
    n = new_usbs();
    if(n > 0){
      printf("%s\n", message);
      printf("\n\nNEW USB DETECTED, TOTAL IS: %d\n", n);
      // handle the new usb
      if(find_new_usb(full_name, part_name) && add_request(part_name)){
          printf("\n+USB: %s, TOTAL: %d\n", part_name, plugged_usbs+n);         
      }else{
          printf("\n[/] USB IGNORED, CURRENT REQ_LIST:\n");
          show_info();
      }
      plugged_usbs += n;
      get_names_list(path, &names_list);
      add_to_map(full_name, part_name);
      show_info();
    }else if(n < 0){
      find_unplugged_usb(full_name, part_name);
      printf("UNPLUGGED NAME FOUND: %s\n", part_name);
      remove_request(part_name);
      plugged_usbs = get_usbs_number();
      get_names_list(path, &names_list);
      printf("\n-USB: idk yet, TOTAL: %d\n", plugged_usbs);
      show_info(); 
    }
    // check for current requests and elaborate them
    //
  }
}

int main(){


  plugged_usbs = get_names_list(path, &names_list); 

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
