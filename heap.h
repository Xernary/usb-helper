#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_NUM 20

void create_array(int n, int array[n]){
  for(int i = 0; i < n; i++)
    array[i] = (rand() % (MAX_NUM-1)) + 1; 
}

void show(int n, const int array[n]){
  for(int i = 0; i < n; i++){
    printf("%d ", array[i]);
  }
  printf("\n\n");
}

void swap(int* a, int* b){
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void sift_up(int* heap, int i){
  int parent = (i-1)/2;

  while(i != 0 && heap[parent] > heap[i]){
    swap(&heap[i], &heap[parent]);
    i = parent;
    parent = (i-1)/2;
  }
}

void sift_down(int* heap, int i, int len){
  int left = i*2 + 1;
  int right = i*2 + 2;

  while((left < len && heap[i] > heap[left]) || 
        (right < len && heap[i] > heap[right])){
    int smallest = ((right >= len || heap[left] < heap[right]) ? left : right);
    swap(&heap[i], &heap[smallest]);
    i = smallest;
    left = i*2 + 1;
    right = i*2 + 2;
  }
}

void heapify(int n, int array[n]){
  for(int i = n-1; i >= 0; i--){
    sift_down(array, i, n);
  }  
}

int extract(int n, int array[n]){
  
  if(n <= 0) return -1;

  int root = array[0];

  array[0] = array[n-1];
  n--;
  sift_down(array, 0, n);

  return root;
}

int* insert(int n, int array[n], int val){

  int* new_heap = malloc(sizeof(int) * ++n);
  new_heap[n-1] = val;

  for(int i = 0; i < n-1; i++){
    new_heap[i] = array[i];
  }

  sift_up(new_heap, n-1);

  return new_heap;
}

int* heapsort(int n, int array[n]){
  int* sorted = malloc(sizeof(int) * n);
  int i = 0;
  while(n > 0){
    sorted[i] = extract(n, array);
    n--;
    i++;
  }
  return sorted;
}



/*int main(int argn, char* args[]){

  if(argn != 2){
    printf("insert array size\n");
    exit(1);
  }

  srand(time(NULL));

  int n = atoi(args[1]);

  int initial_array[n];
  create_array(n, initial_array); 
  show(n, initial_array);

  heapify(n, initial_array);
  show(n, initial_array);





  //int* new_heap = insert(n, initial_array, 3);
  //n++;
  //show(n, new_heap);
  //printf("extracted: %d\n", extract(n, initial_array));
  //n--;
  //show(n, initial_array);

  //int* sorted = heapsort(n, new_heap);
  //show(n, sorted);
}*/




