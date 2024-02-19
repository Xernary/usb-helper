#include "heap.h"
#include <dirent.h>
#include <string.h>


const char* const path = "/dev/disk/by-path";
const char* const substring = "usb";
int current_usbs = 0;

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

  if(strstr(name, substring) != NULL && dirent->d_type == DT_LNK && 
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
  if(n <= 0){
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



void start_detector(){
  int n;
  while(1){
    if((n = get_usbs_number()) != current_usbs){
      printf("%s", "[+] new usb detected\n");
      // handle the new usb
      current_usbs = n;
    }
  }
}


int main(){

  int fd;
  char** names_list;  
  test_heap(10);

  current_usbs = get_names_list(&names_list); 

  printf("n: %d\n\n", current_usbs);

  //start_detector();

  for(int i = 0; i < current_usbs; i++){
    printf("%s\n", names_list[i]);
    free(names_list[i]);
  }
  free(names_list);
  

  printf("%s", "\nworked\n");
  return 0;
}
