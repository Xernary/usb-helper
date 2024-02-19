#include "heap.h"
#include <dirent.h>
#include <string.h>


const char* const path = "/dev";


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

  if(dirent->d_type == DT_BLK && strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
    select = 1;

  return select;
}



// gets every device file name inside the folder at fd and returns the pointer to its array
int get_names_list(char*** names){
  
  struct dirent** dirents_list;
  int n;
  char** names_list;

  n = scandir(path, &dirents_list, filter, alphasort);
  if(n == -1){
    perror("Error in scandir");
    exit(1);
  }

  names_list = malloc(sizeof(char) * n);

  for(int i = 0; i < n; i++){
    //printf("%s\n", dirents_list[i]->d_name);

    names_list[i] = malloc(sizeof(char) * strlen(dirents_list[i]->d_name));
    strcpy(names_list[i], dirents_list[i]->d_name); 

    free(dirents_list[i]);
  }
  free(dirents_list);
  
  *names = names_list;

  return n;
}



int main(){

  int fd;
  char** names_list;  
  int n;
  test_heap(10);

  n = get_names_list(&names_list); 

  printf("n: %d\n\n", n);

  for(int i = 0; i < n; i++){
    printf("%s\n", names_list[i]);
    free(names_list[i]);
  }
  free(names_list);
  

  printf("%s", "\nworked\n");
  return 0;
}
