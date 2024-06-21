/* COP 3502C Assignment 3
 This program was written by: Gabrielle Coronel */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 10
#define NAMESIZE 19

int checkPopcorn(int *perm);
int getsIndexOfName(char* nameCheck);
int status(int *perm, int *used);
void permutation(int *perm, int *used, int n, int maxNumPeople);
void print(int perm[], int maxNumPeople);
int ifOrderSeatingValid (int perm[]);

// initialize number of people attending the movie and pairs of people that don't want to sit next to each other
int n, p;
// list of names
char name[NAMESIZE+1][SIZE+1] = {0};
// list of people who have popcorn
int hasPopcorn[SIZE+1] = {0};
// two dimensional array of people who can't sit next to each other
int cantSitNextTo[SIZE+1][SIZE+1] = {0};

// checkPopcorn function checks if has_popcorn is 0 or 1
int checkPopcorn(int *perm)
{
    // for loop runs through the number of people attending the movie
    for (int i = 0; i < n; i++)
    {
        // inititalize the current index to perm[i]
        int current = perm[i];
        
        // if the current index of the hasPopcorn array is 1
        if (hasPopcorn[current] == 1)
        {
            continue;
        }
        // if the left index or the right index of the array hasPopcorn is 1
        if ((i - 1 >= 0 && hasPopcorn[perm[i-1]] == 1) || (i + 1 < n && hasPopcorn[perm[i+1]] == 1))
        {
            continue;
        }
        // statement is false
        return 0;
    }
    // statement is true
    return 1;
}

// getsIndexOfName function orders the seating without conflict
int getsIndexOfName(char* nameCheck)
{
    // for loop runs through the number of people attending the movie
    for (int i = 0; i < n; i++)
    {
        // if the name in the name array is the same as nameCheck
        if (strcmp(name[i], nameCheck)==0)
        {
            // return the index of the name
            return i;
        }
    }
    return -1;
}

// ifOrderSeatingValid function checks if the order of seating is valid
int ifOrderSeatingValid (int *perm)
{
    // for loop runs through the number of people attending the movie
    for (int i = 0; i < n; i++)
    {
        // inititalize current index to perm[i]
        int current = perm[i];
        // if the index to the left of i is greater or equal to 0
        if (i - 1 >= 0)
        {
            // initialize left index
            int left = perm[i-1];
            // calls cantSitNextTo function to check if the index to the left of current is 1
            if (cantSitNextTo[current][left] == 1)
                return 0;
        }
        // if the index to the right of i is less than the number of people
        if (i + 1 < n)
        {
            // initialize right index
            int right = perm[i+1];
            // calls cantSitNextTo function to check if the index to the right of current is 1
            if (cantSitNextTo[current][right] == 1)
            {
                return 0;
            }
        }
    }
    // returns 1 if both checkPopcorn and cantSitNextTo return 0
    return 1;
}

// status function gets the status of checkPopcorn function and ifOrderSeatingValid function to check if permutation is valid
int status(int *perm, int *used)
{
    // if both conditions of checkPopcorn function and ifOrderSeatingValid function are true
    if ((checkPopcorn(perm)==1) && ifOrderSeatingValid(perm)==1)
    {
        // permutation is valid
        return 1;
    }
    // either condition is false
    return 0;
}

// the used array keeps track of which number is used and which number to be transferred to the perm
void permutation(int perm[], int used[], int n, int maxNumPeople)
{
    // if n is the same as the max number of people
    if (n == maxNumPeople)
    {
        // if status function is 1
        if (status(perm, used)==1)
        {
            // for loop runs through the max number of people
            for (int i = 0; i < maxNumPeople; i++)
            {
                printf("%s\n", name[perm[i]]);
            }
            // permutation ends after the first possible permutation
            exit(0);
        }
    }
    // for loop runs through the max number of people
    for (int i = 0; i < maxNumPeople; i++)
    {
        // if i was not used
        if(!used[i])
        {
            // mark that it is used
            used[i] = 1;
            // transfer i to the perm array at nth position
            perm[n] = i;
            // increaese n and grow further
            permutation(perm, used, n+1, maxNumPeople);
            // unmark i for next process
            used[i] = 0;
            
        }
    }
}

int main(void)
{
    
    // initialize perm to build and store the permutations
    int perm[SIZE] = {0};
    // initialize used to track which index is used
    int used[SIZE] = {0};
    // getting input from the user about the number of people attending the movie and pairs of people that don't want to sit next to each other
    scanf("%d %d", &n, &p);
    // inititalize the number that determines if the person has popcorn or not
    int determinePopcorn;
    // for loop runs through the number of people attending the movie
    for (int i = 0; i < n; i++)
    {
        // getting input from the user about the person's name and their popcorn number
        scanf("%s %d", name[i], &determinePopcorn);
        // checks if determinePopcorn is 1
        if (determinePopcorn == 1)
        {
            // the value at the index of hasPopcorn is 1, meaning that the person has popcorn
            hasPopcorn[i] = 1;
        }
    }
    // for loop runs through the number of people that don't want to sit next to each other
    for (int i = 0; i < p; i++)
    {
        // array of max length and max size
        char name1[NAMESIZE+1], name2[NAMESIZE+1];
        // getting input from the user about the pairs of people that don't want to sit next to each other
        scanf("%s %s", name1, name2);
        // initialize the indexes 1 and 2
        int index1 = getsIndexOfName(name1);
        int index2 = getsIndexOfName(name2);
        // two dimensional array of people who can't sit next to each other
        cantSitNextTo[index1][index2] = 1;
        cantSitNextTo[index2][index1] = 1;
    }
    // calls permutation function
    permutation(perm, used, 0, n);
    return 0;
}
