#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 3000
#define ABS(a) (((a)>0)?(a):(-(a)))

typedef struct heap {
  int *arr;
  int len;
} Heap;

void heap_init(Heap *h, int len) {
  h->len = 0;
  h->arr = (int *)calloc(len, sizeof(int));
}

int heap_is_empty(Heap *h) {
  return h->len == 0;
}

void heap_free(Heap *h) {
  free(h->arr);
}

void siftDown(Heap *h, int start);

void heapify(Heap *h) {
  //heap[] is pointer to heap array
  //len is heap element count
  int start = (h->len-2)/2; //last parent node

  while (start >= 0) {
    siftDown(h, start);
    start--;
  }
}

void siftDown(Heap *h, int start) {
  int root = start;
  while (root*2+1 < h->len) { //until root has one left child
    //check heap property
    int l_index = root*2+1;
    int r_index = l_index+1;
    int swap = root;

    if (h->arr[l_index] > h->arr[swap]) {
      swap = l_index;
    }

    if (r_index < h->len && h->arr[r_index] > h->arr[swap]) {
      swap = r_index;
    }

    if (root == swap) {
      return ;
    } else {
      int t = h->arr[root];
      h->arr[root] = h->arr[swap];
      h->arr[swap] = t;

      root = swap;
    }
  }
}

void heap_insert(Heap *h, int item) {
  h->arr[h->len] = item;
  h->len++;
  heapify(h);
}

int heap_remove(Heap *h) {
  int i = h->arr[0];
  int t = h->arr[h->len-1];
  h->arr[0] = h->arr[h->len-1];
  h->arr[h->len-1] = t;
  h->len -= 1;

  heapify(h);
  return i;
}

int main() {
  int n; //# of input numbers
  int i; //loop var
  int n1; //former number
  int n2; //later number
  int d; //difference between two numbers
  Heap h;
  while(scanf("%d", &n) != EOF) {
    heap_init(&h, n-1);
    scanf("%d", &n1);
    for (i=1; i<n; i++) {
      scanf("%d", &n2);
      d = ABS(n2 - n1);
      n1 = n2;
      /*printf("insert %d\n", d);*/
      heap_insert(&h, d);
    }

    /*for (i=0; i<n-1; i++)*/
      /*printf("heap : %d\n", h.arr[i]);*/

    d = n-1;
    while(!heap_is_empty(&h)) {
      i = heap_remove(&h);
      /*printf("heap returns %d, size = %d\n", i, h.len);*/
      if (d != i) break;
      d--;
    }

    if (d == 0)
      printf("Jolly\n");
    else 
      printf("Not jolly\n");

    heap_free(&h);
  }

  return 0;
}
