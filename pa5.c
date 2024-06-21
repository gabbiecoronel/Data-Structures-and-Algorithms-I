/* COP 3502C Assignment 5
 This program is written by: Gabrielle Coronel */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 19

typedef struct customer {
    char name[MAXLEN+1];
    int points;
} customer;

typedef struct treenode {
    customer* cPtr;
    int size;
    struct treenode* left;
    struct treenode* right;
} treenode;

treenode* createNode(char name[]);
treenode* insert(treenode *root, char name[], int points, int *customerCount);
void subtractPoints(treenode* root, char name[], int points);
treenode* delete(treenode *root, char name[], int *customerCount);
void searchCustomer(treenode* root, char name[], int depth);
int count_smaller(treenode* root, char name[]);
void mergeSort(customer** Customer, int left, int right);
void merge(customer** Customer, int left, int mid, int right);
int compare(customer* L, customer* R);
treenode* maxVal(treenode* root);
void fillCustomerArray(treenode *tree, customer **array, int *position);
void freeTree(treenode* tree);

// createNode function creates nodes for the binary search tree
treenode* createNode(char name[])
{
    // dynamically allocating memory for the temp node
    treenode* temp = malloc(sizeof(treenode));
    // dynamically allocating memory for cPtr
    temp->cPtr = malloc(sizeof(customer));
    // strcpy copies the name inputted by the user into the struct
    strcpy(temp->cPtr->name, name);
    // set the temp node to the left to NULL
    temp->left = NULL;
    // set the temp node to the right to NULL
    temp->right = NULL;
    // set the temp node's size to 1
    temp->size = 1;
    // returns a pointer to the created temp node
    return temp;
}

// insert function adds the customer's name into the binary search tree
treenode* insert(treenode *root, char name[], int points, int *customerCount)
{
    // checks if there are no current customers in the binary tree
    if (root == NULL)
    {
        // calls createNode function to create nodes of customers
        root = createNode(name);
        // set the points from the struct to points inputted
        root->cPtr->points = points;
        // add 1 to the customerCount
        (*customerCount) += 1;
        printf("%s %d\n", root->cPtr->name, root->cPtr->points);
        return root;
    }
    // if there are customers already in the binary tree
    else
    {
        // if the name in the customer struct is the same as the name inputted by the user
        if (strcmp(name, root->cPtr->name)==0)
        {
            // adds the points of the customer with the given points
            root->cPtr->points += points;
            printf("%s %d\n", root->cPtr->name, root->cPtr->points);
            return root;
        }
        // checks if the name in the customer struct is less than the name inputted by the user
        else if (strcmp(name, root->cPtr->name)<0)
        {
            // recursively call the add function fot the left node
            root->left = insert(root->left, name, points, customerCount);
        }
        // if the name in the customer struct is more than the name inputted by the user
        else
        {
            // recursively call the add function for the right node
            root->right = insert(root->right, name, points, customerCount);
        }
    }
    int sizeL = 0, sizeR = 0;
    // if the left of root is not empty
    if (root->left != NULL)
    {
        sizeL = root->left->size;
    }
    // if the right of root is not empty
    if (root->right != NULL)
    {
        sizeR = root->right->size;
    }
    // the size of root is the sum of the left size plus the right size plus 1
    root->size = sizeL + sizeR + 1;
    return root;
}

// sub function subtracts a customer's points
void subtractPoints(treenode* root, char name[], int points)
{
    // checks if there are no current customers in the binary tree
    if (root == NULL)
    {
        printf("%s not found\n", name);
        return;
    }
    // if there are customers already in the binary tree
    else
    {
        if (strcmp(name, root->cPtr->name)==0)
        {
            // subtract the total number of points the customer has
            root->cPtr->points -= points;
            if (root->cPtr->points < 0)
            {
                root->cPtr->points = 0;
            }
            printf("%s %d\n", root->cPtr->name, root->cPtr->points);
        }
        else if (strcmp(name, root->cPtr->name)<0)
        {
            // recursively call the add function fot the left node
            subtractPoints(root->left, name, points);
        }
        else
        {
            // recursively call the add function for the right node
            subtractPoints(root->right, name, points);
        }
    }
}

// delete function deletes the customer's name from the binary search tree
treenode* delete(treenode *root, char name[], int *customerCount)
{
    // checks if there are no current customers in the binary tree
    if (root == NULL)
    {
        printf("%s not found\n", name);
        return root;
    }
    // compare compares the current name from the struct to the given name
    int compare = strcmp(name, root->cPtr->name);
    // checks if the given name is smaller than the current name from the struct
    if (compare < 0)
    {
        // recursively calls delete function for the left of root
        root->left = delete(root->left, name, customerCount);
    }
    // checks if the given name is bigger than the current name from the struct
    else if (compare > 0)
    {
        // recursively calls delete function for the right of root
        root->right= delete(root->right, name, customerCount);
    }
    // if the given name is the same as the current name from the struct
    else
    {
        // checks if the left of root is empty
        if (root->left == NULL)
        {
            treenode* right = root->right;
            // frees dynamically allocated memory for the current node's name
            free(root->cPtr);
            // frees dynamically allocated memory for the node
            free(root);
            // returns the right subtree as the new root
            return right;
        }
        // checks if the right of root is empty
        if (root->right == NULL)
        {
            treenode* left = root->left;
            // frees dynamically allocated memory for the current node's name
            free(root->cPtr);
            // returns the right subtree as the new root
            free(root);
            // returns the left subtree as the new root
            return left;
        }
        // calls maxVal to return the maximum value in the left subtree
        treenode* temp = maxVal(root->left);
        char current[MAXLEN+1];
        int points = 0;
        // strcpy copies the name from the maximum value of the customer node from temp to the current node
        strcpy(current, temp->cPtr->name);
        points = temp->cPtr->points;
        // strcpy copies the current node to the name of the customer node from root
        strcpy(root->cPtr->name, current);
        root->cPtr->points = points;
        // calls delete function to delete the current node with the maximum value in the left subtree
        root->left = delete(root->left, current, customerCount);
    }
    
    int sizeL = 0, sizeR = 0;
    // if the left of root is not empty
    if (root->left != NULL)
    {
        sizeL = root->left->size;
    }
    // if the right of root is not empty
    if (root->right != NULL)
    {
        sizeR = root->right->size;
    }
    // the size of root is the sum of the left size plus the right size plus 1
    root->size = sizeL + sizeR + 1;
    return root;
}

// searchCustomer function searches for a customer's name
void searchCustomer(treenode*root, char name[], int depth)
{
    // if there are customers already in the binary search tree
    if (root != NULL)
    {
        // if the name that's being searched is found in the binary search tree
        if (strcmp(name, root->cPtr->name)==0)
        {
            // print the name that's being searched, the number of loyalty points the customer currently has, and the distance of the node the customer that's found from the root node of the binary search tree
            printf("%s %d %d\n", root->cPtr->name, root->cPtr->points, depth);
            return;
        }
        // if the name that's being searched is to the left
        else if (strcmp(name, root->cPtr->name)<0)
        {
            // recursively calls searchCustomer function to search to the left
            searchCustomer(root->left, name, depth+1);
        }
        // if the name that's being searched is to the right
        else
        {
            // recursively calls searchCustomer function to search to the right
            searchCustomer(root->right, name, depth+1);
        }
    }
    // if the customer wasn't found in the binary search tree
    else
    {
        printf("%s not found\n", name);
        return;
    }
}

// count_smaller function calculates the number of names in the binary search tree that comes alphabetically before name
int count_smaller(treenode* root, char name[])
{
    int count = 0;
    // checks if there are no current customers in the binary search tree
    if (root == NULL)
    {
        return 0;
    }
    // if there are customers already in the binary search tree
    int check = strcmp(name, root->cPtr->name);
    int leftChildSize = (root->left != NULL) ? root->left->size : 0;
    // checks if the name in the customer struct comes before <name> alphabetically
    if (check < 0)
    {
        count = count_smaller(root->left, name);
    }
    // checks if the name in the customer struct comes after <name> alphabetically
    else if (check > 0)
    {
        count += leftChildSize+1+count_smaller(root->right, name);
    }
    // if the name is found
    else
    {
        count += leftChildSize;
    }
    return count;
}

// mergeSort function sorts the customers
void mergeSort(customer** Customer, int left, int right)
{
    if (left < right)
    {
        int mid=(left+right)/2;
        mergeSort(Customer, left, mid);
        mergeSort(Customer, mid+1, right);
        // calls merge function to merge the left and right array of customers together
        merge(Customer, left, mid, right);
    }
}

// merge function merges the left and right array of customers into one array based on their points
void merge(customer** Customer, int left, int mid, int right)
{
    int sizeL = mid-left+1;
    int sizeR = right-mid;
    // dynamically allocating memory for the left array customers
    customer** L = malloc(sizeL*sizeof(customer*));
    // dynamically allocating memory for the right array of customers
    customer** R = malloc(sizeR*sizeof(customer*));
    // for loop runs through sizeL
    for (int i = 0; i < sizeL; i++)
    {
        // make the L array at index i equal to the Customer array at index left+i
        L[i]= Customer[left+i];
    }
    // for loop runs through sizeR
    for (int i = 0; i < sizeR; i++)
    {
        // make the R array at index i equal to the Customer array at index mid+1+i
        R[i] = Customer[mid+1+i];
    }
    int i = 0, j = 0, k = left;
    // while loop continues until i isn't less than sizeL and j isn't less than sizeR
    while (i < sizeL && j < sizeR)
    {
        // checks if the left points are greater or equal to the right points
        if (compare(L[i], R[j])>=0)
        {
            // make the Customer array at index k equal to the L array at index i
            Customer[k] = L[i];
            i++;
        }
        // if the right points are is less than the left points
        else
        {
            // make the Customer array at index k equal to the R array at index j
            Customer[k] = R[j];
            j++;
        }
        k++;
    }
    // while loop continues until i isn't less than sizeL
    while (i < sizeL)
    {
        // make the Customer array at index k equal to the L array at index i
        Customer[k] = L[i];
        i++;
        k++;
    }
    // while loop continues until j isn't less than sizeR
    while (j < sizeR)
    {
        // make the Customer array at index k equal to the R array at index j
        Customer[k] = R[j];
        j++;
        k++;
    }
    // free the memory for the left array
    free(L);
    // free the memory for the right array
    free(R);
}

// compare function compares the left points and the right points
int compare(customer* L, customer* R)
{
    // checks if the left points and the right points are the same
    if (L->points == R->points)
    {
        return strcmp(R->name, L->name);
    }
    // if the left points and right points aren't the same
    else
    {
        return L->points - R->points;
    }
}

// maxVal function finds the maximum value in the binary search tree
treenode* maxVal(treenode* root)
{
    // checks if the right of the root is empty
    if (root->right == NULL)
    {
        // returns the maximum value
        return root;
    }
    // if the right of the root is not empty
    else
    {
        // recursively calls maxVal to the right of the subtree
        return maxVal(root->right);
    }
}

// fillCustomerArray function fills the customer array after all the commands have been completed
void fillCustomerArray(treenode *tree, customer **array, int *position)
{
    // if the binary search tree is empty
    if (tree == NULL)
    {
        return;
    }
    // if the left of the binary search tree is not empty
    if (tree->left != NULL)
    {
        // recursively calls fillCustomerArray to fill the customer array for the right of the tree
        fillCustomerArray(tree->left, array, position);
    }
    array[(*position)++] = tree->cPtr;
    // if the right of the binary search tree is not empty
    if (tree->right != NULL)
    {
        // recursively calls fillCustomerArray to fill the customer array for the left of the tree
        fillCustomerArray(tree->right, array, position);
    }
}

// freeTree function frees the dynamic memory allocated to the binary search tree
void freeTree(treenode* tree)
{
    // checks if the binary search tree is empty
    if (tree == NULL)
    {
        return;
    }
    // if the left of the binary search tree is not empty
    if (tree->left != NULL)
    {
        // recursively call freeTree function for the left of the tree to free dynamic memory allocated to binary search tree
        freeTree(tree->left);
    }
    // if the right of the binary search tree is not empty
    if (tree->right != NULL)
    {
        // recursively call freeTree function for the right of the tree to free dynamic memory allocated to binary search tree
        freeTree(tree->right);
    }
    free(tree);
}

int main(void)
{
    treenode *root = NULL;
    int n, points, customerCount = 0, position = 0;
    char name[MAXLEN+1], command[MAXLEN+1];
    scanf("%d", &n);
    // for loop runs through the number of commands
    for (int i = 0; i < n; i++)
    {
        scanf("%s %s", command, name);
        // if the command is add or sub
        if (strcmp(command, "add")==0 || strcmp(command, "sub")==0)
        {
            scanf("%d", &points);
            // if the command is add
            if (strcmp(command, "add")==0)
            {
                // calls insert function to insert the customer's name and their points into the binary search tree
                root = insert(root, name, points, &customerCount);
            }
            // if the command is sub
            else
            {
                // calls the subtractPoints function to subtract the customer's points from their given points
                subtractPoints(root, name, points);
            }
        }
        // if the command is del
        else if (strcmp(command, "del")==0)
        {
            // stores the size before
            int before;
            // checks if root is empty
            if (root == NULL)
            {
                before = 0;
            }
            // if root isn't empty
            else
            {
                before = root->size;
            }
            // calls the delete function to delete a customer from the binary search tree
            root = delete(root, name, &customerCount);
            // stores the size after
            int after;
            // checks if root is empty
            if (root == NULL)
            {
                after = 0;
            }
            // if root isn't empty
            else
            {
                after = root->size;
            }
            // checks if the size from before is different from the size after
            if (before != after)
            {
                printf("%s deleted\n", name);
            }
        }
        // if the command is search
        else if (strcmp(command, "search")==0)
        {
            // calls the searchCustomer function to search for a customer in the binary search tree
            int depth = 0;
            searchCustomer(root, name, depth);
        }
        // if the command is count_smaller
        else if (strcmp(command, "count_smaller")==0)
        {
            // calls the count_smaller function to
            int count = count_smaller(root, name);
            printf("%d\n", count);
        }
    }
    // checks if the binary tree is empty
    if (root == NULL)
    {
        return 0;
    }
    customerCount = root->size;
    // dynamically allocating memory for customers
    customer** Customer = malloc(customerCount*sizeof(customer*));
    // calls fillCustomerArray function to fill the final customer array after all the commands have been completed
    fillCustomerArray(root, Customer, &position);
    // calls merge sort function to merge
    mergeSort(Customer, 0, customerCount-1);
    // for loop runs through the number of customers
    for (int i = 0; i < customerCount; i++)
    {
        printf("%s %d\n", Customer[i]->name, Customer[i]->points);
    }
    // for loop runs through the number of customers
    for (int i = 0; i < customerCount; i++)
    {
        free(Customer[i]);
    }
    // free memory of Customer
    free(Customer);
    // calls freeTree function to free memory of the binary search tree
    freeTree(root);
    return 0;
}
