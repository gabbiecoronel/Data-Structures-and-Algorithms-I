/* COP 3502C Assignment 4
 This program is written by: Gabrielle Coronel */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPS 0.001

typedef struct Group {
    int groupSize;
    double angleRadians;
    int groupNumber;
} Group;

typedef struct Result {
    int firstGroup;
    int secondGroup;
    double angleDegrees;
} Result;

int doubleCompare(double d1, double d2, double eps);
int compareTo(Result *ptrPt1, Result *ptrPt2);
void merge(Group *group, int left, int mid, int right);
void mergeSort(Group *group, int left, int right);
void quicksort(Result result[], int low, int high);
int partition(Result result[], int low, int high);
void swap(Result* a, Result* b);

// doubleCompare function compares two double type of data and checks whether they are same or not
int doubleCompare(double d1, double d2, double eps)
{
    // checks if the float absolute value of d1 - d2 is less than the epsilon
    if (fabs(d1-d2) < eps)
    {
        // d1 and d2 are same based on the epsilon
        return 1;
    }
    return 0;
}

// compareTo function takes in two pointers, ptrPt1 and ptrPt2, to Result structs
int compareTo(Result *ptrPt1, Result *ptrPt2)
{
    // if the first result of the first group isn't the same as the second result of the first group
    if (ptrPt1->firstGroup != ptrPt2->firstGroup)
    {
        return ptrPt1->firstGroup - ptrPt2->firstGroup;
    }
    else
    {
        return ptrPt1->secondGroup - ptrPt2->secondGroup;
    }
}

// merge function merges the left and right together
void merge(Group *group, int left, int mid, int right)
{
    int sizeL = mid-left+1;
    int sizeR = right-mid;
    // Dynamically alloactes memory and creates an array of the left
    Group *L = malloc(sizeL*sizeof(Group));
    // Dynamically alloactes memory and creates an array of the right
    Group *R = malloc(sizeR*sizeof(Group));
    // for loop runs through the size of L array
    for (int i = 0; i < sizeL; i++)
    {
        // make the L array at index i equal to the group array at index left+i
        L[i] = group[left+i];
    }
    // for loop runs through the size of R array
    for (int i = 0; i < sizeR; i++)
    {
        // make the R array at index i equal to the group array at index mid+1+i
        R[i] = group[mid+1+i];
    }
    int i = 0, j = 0, k = left;
    // while loop continues until i isn't less than sizeL and j isn't less than sizeR
    while (i < sizeL && j < sizeR)
    {
        // checks if the angle in radians in the L array at index i is less than the angle in radians in the R array at index j
        if (L[i].angleRadians < R[j].angleRadians)
        {
            // make the group array at index k equal to the L array at index i
            group[k] = L[i];
            i++;
        }
        // if the angle in radians in the R array at index j is less than the angle in radians in the L array at index i
        else
        {
            // make the group array at index k equal to the R array at index j
            group[k] = R[j];
            j++;
        }
        k++;
    }
    // while loop continues until i isn't less than sizeL
    while (i < sizeL)
    {
        // make the group array at index k equal to the L array at index i
        group[k] = L[i];
        i++;
        k++;
    }
    // while loop continues until j isn't less than sizeR
    while (j < sizeR)
    {
        // make the group array at index k equal to the R array at index j
        group[k] = R[j];
        j++;
        k++;
    }
    // frees the memory for dynamically allocated memory for the left
    free(L);
    // frees the memory for dynamically allocated memory for the right
    free(R);
}

// mergeSort function sort the groups based on their angle in radians
void mergeSort(Group *group, int left, int right)
{
    // if the angle in the left index of the group is less than the angle in the right index of the group
    if (left < right)
    {
        int mid = left + (right-left)/2;
        mergeSort(group, left, mid);
        mergeSort(group, mid+1, right);
        // calls merge function to merge the left and right together
        merge(group, left, mid, right);
    }
}

// quicksort function sorts the array of results
void quicksort(Result result[], int low, int high) {
    // checks if low is less than high
    if (low < high) {
        int split = partition(result,low,high);
        quicksort(result,low,split-1);
        quicksort(result,split+1,high);
    }
}

// partition function splits the result array into parts to sort the results
int partition(Result result[], int low, int high) {
    int i, lowpos;
    // Pick a random partition element and swap it into index low
    i = low + rand()%(high-low+1);
    swap(&result[low], &result[i]);
    // Store the index of the partition element
    lowpos = low;
    // Update our low pointer
    low++;
    // Run the partition so long as the low and high counters don't cross
    while (low <= high) {
        // Move the low pointer until we find a value too large for this side and unitl the result from the compare to function isn't less than or equal to 0
        while (low <= high && compareTo(&result[low], &result[lowpos])<=0)
            low++;
        // Move the high pointer until we find a value too small for this side and unitl the result from the compare to function isn't greater than 0
        while (high >= low && compareTo(&result[high], &result[lowpos])>0)
            high--;
        // Now that we've identified two values on the wrong side, swap them
        if (low < high)
            swap(&result[low], &result[high]);
    }
    // Swap the partition element into it's correct location
    swap(&result[lowpos], &result[high]);
    // Return the index of the partition element
    return high;
}

// swap function swaps the results
void swap(Result* a, Result* b)
{
    Result t = *a;
    *a = *b;
    *b = t;
}

// createAngle function creates an array of results of the angles in degrees
Result* createAngle(Group *group, int numGroups) {
    // Dynamically allocating memory for resultArray
    Result *resultArray = malloc(numGroups*sizeof(Result));
    // for loop runs through the number of groups
    for (int i = 0; i < numGroups; i++)
    {
        // the result of the first group is equal to the position at index i
        resultArray[i].firstGroup = group[i].groupNumber;
        // the result of the second group is equal to the position at index i+1
        resultArray[i].secondGroup = group[(i+1)%numGroups].groupNumber;
        // checks if the first group at index i in the result array is greater than the second group at index i in the result array
        if (resultArray[i].firstGroup > resultArray[i].secondGroup)
        {
            // swap the first group and second group
            int t = resultArray[i].firstGroup;
            resultArray[i].firstGroup = resultArray[i].secondGroup;
            resultArray[i].secondGroup = t;
        }
        // to find the angle from the two groups, subtract the second group's angle by the first group's angle then converting the angle in radians to degrees
        resultArray[i].angleDegrees = (group[(i+1)%numGroups].angleRadians - group[i].angleRadians)*180/M_PI;
        resultArray[i].angleDegrees = fmod((resultArray[i].angleDegrees+360),360);
    }
    return resultArray;
}

int main(void) {
    int group_size, angle_degrees;
    // getting input of the number of groups and the angle in degrees
    scanf("%d %d", &group_size, &angle_degrees);
    // Dynamically allocate group and result
    Group *group = malloc(group_size*(sizeof(Group)));
    int x, y, s;
    // for loop runs through the number of groups
    for (int i = 0; i < group_size; i++)
    {
        // getting input of the x position, y position, and number of people for each group
        scanf("%d %d %d", &x, &y, &s);
        // formula used to calculate the angle in radians using the x and y position given
        double angle = atan2(y, x);
        // for each index in the group array for groupSize is equal to s
        group[i].groupSize = s;
        // for each index in the group array for angleRadians is equal to angle
        group[i].angleRadians = angle;
        // for each index in the group array for groupNumber is equal to the index of i
        group[i].groupNumber = i;
    }
    // calls mergeSort function to sort the groups based on their angle in radians
    mergeSort(group, 0, group_size-1);
    // calls createAngle function to convert the angle in radians to angle in degress
    Result* result = createAngle(group, group_size);
    double bestAngle = 0;
    // for loop runs through the number of groups
    for (int i = 0; i < group_size; i++)
    {
        // if the index in result array for angleDegrees is more than the bestAngle
        if (result[i].angleDegrees > bestAngle)
        {
            // make bestAngle equal to the index in result array for angleDegrees
            bestAngle = result[i].angleDegrees;
        }
        
    }
    // calls quicksort function to sort the array of results
    quicksort(result, 0, group_size-1);
    printf("Max projection degree without harming %4.4lf\n", bestAngle);
    printf("Closest possible group pairs in order:\n");
    // for loop runs through the number of groups
    for (int i = 0; i < group_size; i++)
    {
        // calls doubleCompare function to compare the result in angle degrees and the best angle
        if (doubleCompare(result[i].angleDegrees, bestAngle, EPS))
        {
            printf("%d %d\n", result[i].firstGroup, result[i].secondGroup);
        }
    }
    // frees the memory for dynamically allocated memory for group
    free(group);
    // frees the memory for dynamically allocated memory for result
    free(result);
    return 0;
}
