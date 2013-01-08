#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int partition(float a[], int b[], int startIndex, int endIndex) {
    int random = rand()%(endIndex-startIndex)+startIndex;
    float pivot = a[random];
    a[random] = a[endIndex];
    a[endIndex] = pivot;
    
    int leftIndex = startIndex;
    for(int i=startIndex; i<endIndex; i++) {
            if(a[i] < pivot) {
                        float temp2 = a[i];
                        a[i] = a[leftIndex];
                        a[leftIndex] = temp2;

                        int temp3 = b[i];
                        b[i] = b[leftIndex];
                        b[leftIndex] = temp3;

                        leftIndex++;

                    }
        }
    float temp = a[leftIndex];
    a[leftIndex] = a[endIndex];
    a[endIndex] = temp;
    int temp2 = b[leftIndex];
    b[leftIndex] = b[endIndex];
    b[endIndex] = temp2;
    return leftIndex;
}

void quicksort(float array[], int array2[], int startIndex, int endIndex) {
    if(startIndex < endIndex) {
            int pivotIndex = partition(array, array2, startIndex, endIndex);
            quicksort(array, array2, startIndex, pivotIndex-1);
            quicksort(array, array2, pivotIndex+1, endIndex);
        }
}

int find_max(float array[], int start, int end) {
  int max = 0;
  int max_index = 0;
  for(int i=start; i<end; i++) {
    if(array[i]>max) {
      max = array[i];
      max_index = i;
    }
  }
  return max_index;
}

float sum(float array[], int start, int end) {
  float sum = 0;
  for(int i=start; i<end; i++) {
    sum+=array[i];
  }
  return sum;
}

float energy(float array[], int start, int end) {
  float sum = 0;
  for(int i=start; i<end; i++) {
    sum+=abs(array[i])*abs(array[i]);
  }
  return sum;
}

int min(int a, int b) {
  if(a<b)
    return a;
  else 
    return b;
}

float pow(float a, int b) {
  float power = 1;
  for(int i=1; i<=b; i++) {
    power*=a;
  }
  return a;
}
