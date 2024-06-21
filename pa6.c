/* COP 3502C Assignment 6
 This program was written by: Gabrielle Coronel */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 19
#define TABLESIZE 300007

typedef struct item {
    char name[MAXLEN+1];
    int quantity;
    int saleprice;
} item;

typedef struct node {
    item* iPtr;
    struct node* next;
} node;

typedef struct hashtable {
    node** lists;
    int size;
} hashtable;

// linked list functions
node* insert_front(node *front, char* word, int quantity, int totalprice);
void dellist(node *front);
node* search(node* temp, char* word);

// hash table functions
hashtable *initTable(void);
int hashfunc(char* word, int size);
void insertTable(hashtable *hashTable, char* word, int quantity, int totalprice);
node* searchTable(hashtable *hashTable, char* word);
void freeTable(hashtable *hashTable);

// command functions
void buy(hashtable *hashTable, char* word, int quantity, int totalprice, int* totalcash);
void sell(hashtable *hashTable, char* word, int quantity, int* totalcash);
void change_price(hashtable *hashTable, char* word, int change_price);

int complexity = 0;

// hashfunc function calculates the hash value for a word
int hashfunc(char* word, int size) {
    int len = strlen(word);
    int res = 0;
    for (int i=0; i<len; i++)
        res = (1151*res + (word[i]-'a'))%size;
    return res;
}

// initTable function initializes the hash table
hashtable *initTable(void)
{
    // dynamically allocating memory for the hashTable
    hashtable *hashTable = malloc(sizeof(hashtable));
    // dynamically allocate memory for the lists in the hash table
    hashTable->lists = malloc(TABLESIZE * sizeof(node*));
    // for loop runs through the size of the hash table
    for (int i = 0; i < TABLESIZE; i++)
    {
        // sets the lists of the hash table for each index to NULL
        hashTable->lists[i] = NULL;
    }
    // set the size of the hash table to the size of the hash table
    hashTable->size = TABLESIZE;
    return hashTable;
}

// insertTable function inserts a word into the hash table
void insertTable(hashtable *hashTable, char* word, int quantity, int totalprice) {
    // calls the hashfunc function to calculate the hashValue for the word
    int hashval = hashfunc(word, TABLESIZE);
    // calls insert_front function to insert a new node to the front of a linked list
    hashTable->lists[hashval] = insert_front(hashTable->lists[hashval], word, quantity, totalprice);
}

// insert_front function inserts a node to the front of a linked list
node* insert_front(node *front, char* word, int quantity, int totalprice) {
    complexity++;
    // dynamically allocating memory for the temporary node
    node *temp = malloc(sizeof(node));
    // dynamically allocating memory for the item being created
    temp->iPtr = malloc(sizeof(item));
    // strcpy copies the word (the item being added to the front of a linked list) into the item struct for name
    strcpy(temp->iPtr->name, word);
    temp->iPtr->quantity = quantity;
    temp->iPtr->saleprice = totalprice;
    temp->next = front;
    // returns the new front of the linked list
    return temp;
}

// searchTable function searches for a word stored in the hash table
node* searchTable(hashtable *hashTable, char* word) {
    // calls the hashfunc function to calculate the hashValue for the word
    int hashval = hashfunc(word, TABLESIZE);
    // returns 1 is the word is stored in the hash table or 0 if it isn't
    return search(hashTable->lists[hashval], word);
}

// search function searches for word in a linked list
node* search(node* temp, char* word)
{
    // while loop continues until it's empty
    while (temp != NULL)
    {
        complexity++;
        // checks if the name of the item and the item being searched is the same
        if (strcmp(temp->iPtr->name, word) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// dellist function frees all the dynamically allocated memory for a linked list
void dellist(node *element) {
    node *temp;
    // checks if the element isn't NULL
    if (element != NULL) {
        // frees the allocated memory for the item pointer from the element in the linked list
        free(element->iPtr);
        temp = element->next;
        // recursively calls dellist to free each element in the list
        dellist(temp);
        // frees the allocated memory for the element in the linked list
        free(element);
    }
}

// freeTable function frees all the dynamcially allocated memory for a hash table
void freeTable(hashtable *hashTable) {
    // set all of these to NULL
    for (int i = 0; i < TABLESIZE; i++) {
        // calls dellist function to free all the memory for a linked list
        dellist(hashTable->lists[i]);
    }
    // frees the allocated memory for the lists in the hash table
    free(hashTable->lists);
    // frees the allocated memory for the hash table
    free(hashTable);
}

// buy function processes the item that's being bought and its quantity
void buy(hashtable *hashTable, char* word, int quantity, int totalprice, int* totalcash)
{
    // calculates the totalcash after the item is bought
    *totalcash -= totalprice;
    // calls the searchTable function to search for a word in the linked list
    node* currentWord = searchTable(hashTable, word);
    // checks if the currentWord that's being searched wasn't found
    if (currentWord == NULL)
    {
        // calls the insertTable function to insert the currentWord in the hash table
        insertTable(hashTable, word, quantity, totalprice);
        // calls the hashfunc function to calculate the hash value
        int hashValue = hashfunc(word, TABLESIZE);
        currentWord = hashTable->lists[hashValue];
    }
    else
    {
        currentWord->iPtr->quantity += quantity;
    }
    
    printf("%s %d %d\n", word, currentWord->iPtr->quantity, *totalcash);
}

// sell function processes the item that's being sold and its quantity
void sell(hashtable *hashTable, char* word, int quantity, int* totalcash)
{
    // calls the searchTable function to search for a word in the linked list
    node* currentWord = searchTable(hashTable, word);
    // checks if the quantity to sell is smaller or equal to the current quantity of the current word in the item struct
    if (quantity <= currentWord->iPtr->quantity)
    {
        currentWord->iPtr->quantity -= quantity;
        // calculates the total cash after the item is sold
        *totalcash += quantity * currentWord->iPtr->saleprice;
    }
    // if the quantity to sell is bigger than the current quantity of the current word in the item struct
    else
    {
        // calculates the total cash after the item is sold
        *totalcash += currentWord->iPtr->quantity * currentWord->iPtr->saleprice;
        currentWord->iPtr->quantity = 0;
    }
    printf("%s %d %d\n", word, currentWord->iPtr->quantity, *totalcash);
}

// change_price function changes the price of an item
void change_price(hashtable *hashTable, char* word, int newPrice)
{
    // calls the searchTable function to search for a word in the linked list
    node* currentWord = searchTable(hashTable, word);
    // update the saleprice in the item struct to the new price
    currentWord->iPtr->saleprice = newPrice;
}

int main(void) {
    int n, totalcash = 100000;
    char command[MAXLEN+1], word[MAXLEN+1];
    scanf("%d\n", &n);
    // calling the initTable function to initialize the hashTable
    hashtable* hashTable = initTable();
    // for loop runs through the number of commands
    for (int i = 0; i < n; i++) {
        scanf("%s %s", command, word);
        // checks if the command is buy or sell
        if (strcmp(command, "buy")==0 || strcmp(command, "sell")==0)
        {
            int quantity;
            scanf("%d", &quantity);
            // checks if the command is buy
            if (strcmp(command, "buy")==0)
            {
                int totalprice;
                scanf("%d", &totalprice);
                // calls the buy function to buy an item
                buy(hashTable, word, quantity, totalprice, &totalcash);
            }
            // if the command is sell
            else
            {
                // calls the sell function to sell an item
                sell(hashTable, word, quantity, &totalcash);
            }
        }
        // checks if the command is change_price
        else if (strcmp(command, "change_price")==0)
        {
            int newPrice;
            scanf("%d", &newPrice);
            // calls the change_price function to change the price of an item
            change_price(hashTable, word, newPrice);
        }
    }
    printf("%d\n%d\n", totalcash, complexity);
    // calls freeTable function to free the dynamically allocated memory for the hash table
    freeTable(hashTable);
    return 0;
}
