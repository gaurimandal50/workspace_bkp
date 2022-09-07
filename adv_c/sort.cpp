#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <time.h>
using namespace std;
using namespace std::chrono;

int verify_sort(int *arr, int size) {

    for(int i = 0; i < size -1; i++)
        if(arr[i] > arr[i+1]) {
            cout <<"Sorting error"<<endl;
            return -1;
        }

    return 0;
}

struct bucket
{
    int count;
    int* value;
};

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int getMax(int array[], int size) {
   int max = array[1];
   for(int i = 2; i<=size; i++) {
      if(array[i] > max)
         max = array[i];
   }
   return max; //the max element from the array
}

void count_sort(int *array, int size) {
   int *output = new int[size];
   int *head_output = output;
   int max = getMax(array, size);
   int *count = new int[max+1]; //create count array (max+1 number of elements)
   int *head = count;

   for(int i = 0; i < size; i++)
       output[i] = 0; //emptying array to all zero
   for(int i = 0; i<=max; i++)
      count[i] = 0; //initialize count array to all zero
   for(int i = 0; i <size; i++)
      count[array[i]]++; //increase number count in count array.
   for(int i = 0; i <= max; i++)
      count[i+1] += count[i]; //find cumulative frequency

   for(int i = 0; i < size; i++) {
      output[count[array[i]]-1] = array[i];
      count[array[i]]--;
   }

   for(int i = 0; i < size; i++) {
      array[i] = output[i]; //store output array to main array
   }
   delete(head);
   delete(head_output);
}

void countSort(int *arr, int n, int exp)
{
    int output[n]; // output array
    int i, count[10] = { 0 };

    // Store count of occurrences in count[]
    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

// The main function to that sorts arr[] of size n using
// Radix Sort
void radix_sort(int *arr, int n)
{
    // Find the maximum number to know number of digits
    int m = getMax(arr, n);

    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

int compare_integers(const void* first, const void* second)
{
    int x = *((int*)first), y =  *((int*)second);
    if (x == y)
    {
        return 0;
    }
    else if (x < y)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void bucket_sort(int *array ,int n)
{
    struct bucket buckets[3];
    int i, j, k;
    for (i = 0; i < 3; i++)
    {
        buckets[i].count = 0;
        buckets[i].value =new int[n];
    }

    for (i = 0; i < n; i++)
    {
        if (array[i] < 0)
        {
            buckets[0].value[buckets[0].count++] = array[i];
        }
        else if (array[i] > 10)
        {
            buckets[2].value[buckets[2].count++] = array[i];
        }
        else
        {
            buckets[1].value[buckets[1].count++] = array[i];
        }
    }
    for (k = 0, i = 0; i < 3; i++)
    {
        // now using quicksort to sort the elements of buckets
        qsort(buckets[i].value, buckets[i].count, sizeof(int), &compare_integers);
        for (j = 0; j < buckets[i].count; j++)
        {
            array[k + j] = buckets[i].value[j];
        }
        k += buckets[i].count;
        delete(buckets[i].value);
    }
}

void heapify(int arr[], int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// main function to do heap sort
void heap_sort(int *arr, int size)
{
    // Build heap (rearrange array)
    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(arr, size, i);

    // One by one extract an element from heap
    for (int i = size - 1; i > 0; i--) {
        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

int partition (int arr[], int low, int high)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quick_sort(int *arr, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

void merge(int array[], int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    auto *leftArray = new int[subArrayOne],
         *rightArray = new int[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void merge_sort(int *arr, int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively

    auto mid = begin + (end - begin) / 2;
    merge_sort(arr, begin, mid);
    merge_sort(arr, mid + 1, end);
    merge(arr, begin, mid, end);
}

void shell_sort(int *arr, int size)
{
    // Start with a big gap, then reduce the gap
    for (int gap = size/2; gap > 0; gap /= 2)
    {
        // Do a gapped insertion sort for this gap size.
        // The first gap elements a[0..gap-1] are already in gapped order
        // keep adding one more element until the entire array is
        // gap sorted
        for (int i = gap; i < size; i += 1)
        {
            // add a[i] to the elements that have been gap sorted
            // save a[i] in temp and make a hole at position i
            int temp = arr[i];

            // shift earlier gap-sorted elements up until the correct
            // location for a[i] is found
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];

            //  put temp (the original a[i]) in its correct location
            arr[j] = temp;
        }
    }
}

void cocktail_sort(int *a, int size)
{
    bool swapped = true;
    int start = 0;
    int end = size - 1;

    while (swapped)
    {
        // reset the swapped flag on entering
        // the loop, because it might be true from
        // a previous iteration.
        swapped = false;

        // loop from left to right same as
        // the bubble sort
        for (int i = start; i < end; ++i)
        {
            if (a[i] > a[i + 1]) {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }

        // if nothing moved, then array is sorted.
        if (!swapped)
            break;

        // otherwise, reset the swapped flag so that it
        // can be used in the next stage
        swapped = false;

        // move the end point back by one, because
        // item at the end is in its rightful spot
        --end;

        // from right to left, doing the
        // same comparison as in the previous stage
        for (int i = end - 1; i >= start; --i)
        {
            if (a[i] > a[i + 1]) {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }

        // increase the starting point, because
        // the last stage would have moved the next
        // smallest number to its rightful spot.
        ++start;
    }
}

void selection_sort(int *arr, int size)
{
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < size-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < size; j++)
        if (arr[j] < arr[min_idx])
            min_idx = j;

        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

void bubble_sort(int *arr, int size)
{
    int i, j;
    for (i = 0; i < size-1; i++)
        for (j = 0; j < size-i-1; j++)
            if (arr[j] > arr[j+1])
                swap(&arr[j], &arr[j+1]);
}

void insertion_sort(int *arr, int size) {
    int k = 0;
    int tmp = 0;

    for(int i = 1; i < size; i++) {
        for(int j = 0; j < i; j++) {
            if(arr[i] < arr[j]) {
                tmp = arr[i];
                for(k = i; k >= j; k--) {
                    arr[k] = arr[k-1];
                }
                arr[j] = tmp;
                break;
            }
        }
    }
}

/*random_list generates list of random integers of size size between 1 and range_max.
 *Caller must free the array*/
int *random_list(int range_max, int size) {
    int min = 1;
    int tmp = 0;
    time_t tm = {0};

    int *arr = new int[size];
    int *head = arr;
    if(!head) {
        cout<<"Error: new failed."<<endl;
        return NULL;
    }

    srand(time(0));
    for(int i = 0; i < size; i++) {
        while(1) {
            tm = time(NULL);
            tmp = rand()%range_max;
            if((tmp >= min)) {
                *arr = tmp;
                arr++;
                break;
            }
        }
    }


    return head;
}

inline int print_array(int *arr, int size) {
    if(arr) {
        for(int i = 0; i < size; i++) {
            cout<<*(arr+i)<<endl;
        }
    }
    return 0;
}

int main() {
    int range_max = 500;
    int array_size = 10;
    int repeat = 5;
    cout<<"Please enter array_size, range_max and iteration number for wihich the sort algos are to be iterated for getting precise complexity."<<endl;
    cout<<"array_size  ";
    cin>> array_size;
    cout<<endl<<"range_max  ";
    cin>> range_max;
    cout<<endl<<"iteration  ";
    cin>> repeat;
    cout<<endl;

    if(!array_size || !range_max || !repeat) {
        cout<<"You entered invalid numbers"<<endl;
        return -1;
    }

    ofstream myfile;
    clock_t start = {0};
    clock_t stop = {0};
    /*arrN keeps copy of unsorted array of size array_size*/
    int *arr1  = random_list(range_max, array_size);
    int *arr2  = new int[array_size];
    int *arr3  = new int[array_size];
    int *arr4  = new int[array_size];
    int *arr5  = new int[array_size];
    int *arr6  = new int[array_size];
    int *arr7  = new int[array_size];
    int *arr8  = new int[array_size];
    int *arr9  = new int[array_size];
    int *arr10 = new int[array_size];
    int *arr11 = new int[array_size];
    int *arr12 = new int[array_size];

    /*Copying unsorted (original) array to all the buffers*/
    memcpy(arr2,  arr1, sizeof(int)*array_size);
    memcpy(arr3,  arr1, sizeof(int)*array_size);
    memcpy(arr4,  arr1, sizeof(int)*array_size);
    memcpy(arr5,  arr1, sizeof(int)*array_size);
    memcpy(arr6,  arr1, sizeof(int)*array_size);
    memcpy(arr7,  arr1, sizeof(int)*array_size);
    memcpy(arr8,  arr1, sizeof(int)*array_size);
    memcpy(arr9,  arr1, sizeof(int)*array_size);
    memcpy(arr10, arr1, sizeof(int)*array_size);
    memcpy(arr11, arr1, sizeof(int)*array_size);
    memcpy(arr12, arr1, sizeof(int)*array_size);


    myfile.open ("table.txt");
    cout.precision(15);

    start = clock();
    for(int i = 0; i < repeat; i++) {
        bubble_sort(arr1, array_size);
    }

    myfile <<"bubble_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr1, array_size)) {
            cout<<"Error: in bubble sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        selection_sort(arr2, array_size);
    }

    myfile <<"selection_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr2, array_size)) {
            cout<<"Error: in selection sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        insertion_sort(arr3, array_size);
    }

    myfile <<"insertion_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr3, array_size)) {
            cout<<"Error: in insertion sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        cocktail_sort(arr4, array_size);
    }

    myfile <<"cocktail_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr4, array_size)) {
            cout<<"Error: in cocktail sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        shell_sort(arr5, array_size);
    }

    myfile <<"shell_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr5, array_size)) {
            cout<<"Error: in shell sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        merge_sort(arr6, 0, array_size -1);
    }

    myfile <<"merge_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr6, array_size)) {
            cout<<"Error: in merge sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        quick_sort(arr7, 0, array_size -1);
    }

    myfile <<"quick_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr7, array_size)) {
            cout<<"Error: in quick sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        heap_sort(arr8, array_size);
    }

    myfile <<"heap_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr8, array_size)) {
            cout<<"Error: in heap sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        count_sort(arr9, array_size);
    }

    myfile <<"count_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr9, array_size)) {
            cout<<"Error: in count sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        bucket_sort(arr10, array_size);
    }

    myfile <<"bucket_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr10, array_size)) {
            cout<<"Error: in bucket sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        radix_sort(arr11, array_size);
    }
    myfile <<"radix_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr11, array_size)) {
            cout<<"Error: in radix sort failed to sort"<<endl;
            goto err;
    }

    start = clock();
    for(int i = 0; i < repeat; i++) {
        sort(arr12, arr12 + array_size);
    }
    myfile <<"native_sort\t"<< fixed <<  float(float(clock() - start)/CLOCKS_PER_SEC)/repeat  << endl;
    if(verify_sort(arr12, array_size)) {
            cout<<"Error: in native sort failed to sort"<<endl;
            goto err;
    }

    /*printing sorted arrays*/
    cout<<"----------------bubble sort---------------"<<endl;
    print_array(arr1,  array_size);
    cout<<"----------------selection sort------------"<<endl;
    print_array(arr2,  array_size);
    cout<<"----------------insertion sort------------"<<endl;
    print_array(arr3,  array_size);
    cout<<"----------------cocktail sort-------------"<<endl;
    print_array(arr4,  array_size);
    cout<<"----------------shell sort----------------"<<endl;
    print_array(arr5,  array_size);
    cout<<"----------------merge sort----------------"<<endl;
    print_array(arr6,  array_size);
    cout<<"----------------quick sort----------------"<<endl;
    print_array(arr7,  array_size);
    cout<<"----------------heap sort-----------------"<<endl;
    print_array(arr8,  array_size);
    cout<<"----------------count sort----------------"<<endl;
    print_array(arr9,  array_size);
    cout<<"----------------bucket sort---------------"<<endl;
    print_array(arr10, array_size);
    cout<<"----------------radix sort----------------"<<endl;
    print_array(arr11, array_size);
    cout<<"----------------native sort---------------"<<endl;
    print_array(arr12, array_size);

err:
    myfile.close();
    delete(arr1);
    delete(arr2);
    delete(arr3);
    delete(arr4);
    delete(arr5);
    delete(arr6);
    delete(arr7);
    delete(arr8);
    delete(arr9);
    delete(arr10);
    delete(arr11);
    delete(arr12);
    return 0;
}
