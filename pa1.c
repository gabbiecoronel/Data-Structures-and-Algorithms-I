/* COP 3502C Assignment 1
 This program was written by: Gabrielle Coronel */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INTSIZE 10 // initial size of any row (in terms of number orders)
#define MAXLEN 50 // maximum length of a name for an order
#define MAXROWS 100000 // maximum number of rows in the theater

typedef struct order {
    int s_seat;
    int e_seat;
    char* name;
} order;

typedef struct theaterrow{
    order** list_orders;
    int max_size;
    int cur_size;
} theaterrow;

order* make_order(int start, int end, char* this_name);
theaterrow* make_empty_row();
int can_add_order(theaterrow* this_row, order* this_order);
int add_order(theaterrow* this_row, order* this_order);
char* get_owner(theaterrow** theater, int row, int seat_num);
char* get_row_owner(theaterrow* this_row, int seat_num);
int conflict(order* order1, order* order2);
int contains(order* myorder, int seat_no);
void free_order(order* this_order);
void free_row(theaterrow* this_row);

// Returns a pointer to a dynamically allocated order storing the given start row, end row, and the name this_name. Note: strcpy should be used to copy the contents into the struct after its name field is dynamically allocated.
order* make_order(int start, int end, char* this_name)
{
    order* make;
    // dynamically allocating make
    make = (struct order*)malloc(sizeof(struct order));
    // stores the starting row in the struct order
    make->s_seat = start;
    // stores the ending row in the struct order
    make->e_seat = end;
    // dynamically allocating name in the struct order
    make->name = malloc(sizeof(strlen(this_name)*sizeof(char)));
    // strcpy copies this_name (name inputted by the user) into the struct order
    strcpy(make->name, this_name);
    return make;
}

// This function will allocate the memory for one theaterrow, including allocating its array of orders to a default maximum size of 10, and setting its current size to 0.
theaterrow* make_empty_row()
{
    // allocated memory for one theaterrow
    theaterrow* empty = malloc(sizeof(theaterrow));
    // making array of orders to max size of 10
    empty->max_size = INTSIZE;
    // setting current size to 0
    empty->cur_size = 0;
    // allocating memory for the list of orders in theater row and setting it to zero
    empty->list_orders = calloc(INTSIZE, sizeof(order*));
    return empty;
    
}
// Assuming that order1 and order2 point to orders on the same row, this function returns 1 if the orders conflict, meaning that they share at least 1 seat in common, and returns 0 otherwise.
int conflict(order* order1, order* order2)
{
    // If the first order ending seat is less than the second order starting seat (the orders don't have a seat in common)
    if ((order1->e_seat) < (order2->s_seat))
    {
        return 0;
    }
    // If the second order ending seat is less than the first order starting seat (the orders don't have a seat in common)
    if ((order2->e_seat) < (order1->s_seat))
    {
        return 0;
    }
    // If the order has at least one seat in common
    return 1;
}

// Returns 1 if the order pointed to by this_order can be added to the row pointed to by this_row. Namely, 1 is returned if and only if this_order does NOT have any seats in it that are part of any order already on this_row.
int can_add_order(theaterrow* this_row, order* this_order)
{
    // for loop running through the row's current size
    for (int i = 0; i < this_row->cur_size; i++)
    {
        // calls conflict function to see if there are conflicting orders to the same row
        if (conflict(this_row->list_orders[i], this_order))
        {
            // order can't be added because there's conflicting seats on the same row
            return 0;
        }
    }
    // order can be added because there's no conflicting seats on the same row
    return 1;

}

// This function tries to add this_order to this_row. If successful, the order is added and 1 is returned. Otherwise, 0 is returned and no change is made to this_row. If the memory for this_row is full, this function will double the maximum # of orders allocated for the row (via realloc), before adding this_order, and adjust max_size and cur_size of this_row.
int add_order(theaterrow* this_row, order* this_order)
{
    // calls can_add_order function to see if the row can be added to order
    if (can_add_order(this_row, this_order) == 0)
    {
        // order can't be added to the row
        return 0;
    }
    // if the memory for the row is full
    if (this_row->cur_size == this_row->max_size)
    {
        // doubles the maximum number of orders
        this_row->max_size *= 2;
        // resizing the memory to allocate more space to the list of orders for the row
        this_row->list_orders = realloc(this_row->list_orders, this_row->max_size  * sizeof(order*));
    }
    this_row->list_orders[this_row->cur_size] = this_order;
    this_row->cur_size++;
    // order was successfully added
    return 1;
}

// If seat_num seat number in row number row is occupied, return a pointer to the owner’s name. Otherwise, return NULL.
char* get_owner(theaterrow** theater, int row, int seat_num)
{
    return get_row_owner(theater[row], seat_num);
}

// If seat_num in the row pointed to by this_row is occupied, return a pointer to the string storing the owner’s name. If no one is sitting in this seat, return NULL.
char* get_row_owner(theaterrow* this_row, int seat_num)
{
    // for loop runs through the row's current size
    for (int i = 0; i < this_row->cur_size; i++)
    {
        // calls the function contains to see if the seat number in the row from the list orders is occupied
        if(contains(this_row->list_orders[i], seat_num))
        {
            // returns a pointer to the string storing the owner's name
            return this_row->list_orders[i]->name;
        }
    }
    // seat isn't occupied
    return NULL;
}

// This function returns 1 if the seat number seat_no is contained in the range of seats pointed to by myorder, and returns 0 otherwise.
int contains(order* myorder, int seat_no)
{
    // if the seat number chosen is greater than or equal to the starting seat in the order and if the seat number chosen is less than or equal to the ending seat in the order
    if ((seat_no >= myorder->s_seat) && (seat_no <= myorder->e_seat))
    {
        // the seat number is in the range of seats in the order
        return 1;
    }
    // the seat number is not in the range of seats in the order
    return 0;
}

// Frees all memory associated with this_order.
void free_order(order* this_order)
{
    // frees the memory of the names in the entire order
    free(this_order->name);
    // frees the memory of the entire order
    free(this_order);
}

// Frees all memory associated with this_row.
void free_row(theaterrow* this_row)
{
  for (int i = 0; i < this_row->cur_size; i++)
    {
        // Frees the memory of list orders in the row's current size
        free_order(this_row->list_orders[i]);
    }
    // frees the memory of the list of orders in the row
    free(this_row->list_orders);
    // frees the memory of the entire row
    free(this_row);
}

int main(void) {
    // allocates an array called amc where each element in array amc is a pointer to a theater row
    theaterrow** amc = calloc(MAXROWS+1, sizeof(theaterrow*));
    // command has a size of MAXLEN (50) + 1 for the null sentinal ('\0')
    char command[MAXLEN+1];
    // for loop runs through the maximum rows + 1 in the theater
    for (int i = 0; i < MAXROWS+1; i++)
    {
        // creates an empty row for each row in the theater
        amc[i] = make_empty_row();
    }
    // while the user doesn't enter the command "QUIT" the program will keep running
    scanf("%s", command);
    while (strcmp("QUIT", command)!=0)
    {
        // if the user entered the command "BUY"
        if ((strcmp("BUY", command)==0))
        {
            int start_seat, end_seat, row;
            char name[MAXLEN+1];
            scanf("%d %d %d %s", &row, &start_seat, &end_seat, name);
            order* new_order = make_order(start_seat, end_seat, name);
            // calls add_order function to see if the seat in the specific row can't be added to the order
            if (add_order(amc[row-1], new_order)==0)
            {
                // order can't be added
                printf("FAILURE\n");
            }
            else
            {
                // order was added
                printf("SUCCESS\n");
            }
        }
        // if the user entered the command "LOOKUP"
        if (strcmp("LOOKUP", command)==0)
        {
            int row, seat;
            scanf("%d %d", &row, &seat);
            // calls get_row_owner function to see if someone doesn't own the seat in the row
            if (get_row_owner(amc[row-1], seat)==0)
            {
                // no one owns the seat in the row
                printf("No one\n");
            }
            else
            {
                // prints the owner's name based on their row and seat
                printf("%s\n", get_row_owner(amc[row-1], seat));
            }
        }
      scanf("%s", command);
    }
    // frees all the memory associated with amc
    for (int i = 0; i <= MAXROWS; i++)
    {
      free_row(amc[i]);
    }
    // frees the memory that stores the main array (amc)
    free(amc);
    return 0;
}
