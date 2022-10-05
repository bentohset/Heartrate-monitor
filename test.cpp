#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int partition(vector<int>a, int i, int j) {
  // ================== the only addition for Randomized Quick Sort
  int r = i + rand()%(j-i+1); // a random index between [i..j]
  swap(a[i], a[r]); // tada
  // ==================
  int p = a[i]; // p is the pivot
  int m = i; // S1 and S2 are initially empty
  for (int k = i+1; k <= j; ++k) { // explore the unknown region
    if ((a[k] < p) || ((a[k] == p) && (rand()%2 == 0))) { // case 2 (PATCHED solution to avoid TLE O(N^2) on input array with identical values)
      ++m;
      swap(a[k], a[m]); // C++ STL algorithm std::swap
    } // notice that we do nothing in case 1: a[k] > p
  }
  swap(a[i], a[m]); // final step, swap pivot with a[m]
  return m; // return the index of pivot, to be used by Quick Sort
}

void quickSort(vector<int>a, int low, int high) {
  if (low < high) {
    int pivotIdx = partition(a, low, high); // O(N)
    // a[low..high] ~> a[low..pivotIdxâ€“1], pivot, a[pivotIdx+1..high]
    quickSort(a, low, pivotIdx-1); // recursively sort left subarray
    // a[pivotIdx] = pivot is already sorted after partition
    quickSort(a, pivotIdx+1, high); // then sort right subarray
  }
}
int main(){
    vector<int>arr = {1,4,7,9,11,16};
    quickSort(arr,1,16);
    for (int i=0; i<arr.size();i++) cout << arr[i];
    return 0;
}