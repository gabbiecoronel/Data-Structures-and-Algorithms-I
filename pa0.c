/*  COP 3502C Assignment 0
This program is written by: Gabrielle Coronel */

#include <stdio.h>
#include <string.h>
#define size 100002

int main(void) {
    char original_message[size], new_message[size]; // The array original_message[size] holds the string of the original sign and the array new_message[size] holds the string of the new sign
    int frequency_array[26] = {0}, buy_letters = 0; // The arrray frequency_array[26] holds the number of times each letter in the alphabet occurs and buy_letters represents the number of letters that need to be bought
    fgets(original_message, sizeof(original_message), stdin); // Taking the original message inputted by the user and the amount of characters in the original message
    fgets(new_message, sizeof(new_message), stdin); // Taking the new message inputted by the user and the amount of characters in the new message
    for (int i = 0; i < strlen(original_message); i++) // For loop goes through each character of the original string and increments by 1 until the string is done
    {
        if ((original_message[i] >= 'A') && (original_message[i] <= 'Z')) // If the characters in the original message are uppercase and in the range of the alphabet
        {
            frequency_array[original_message[i] - 'A']--; // decreasing the frequency array in the original message by 1 because these letters are going to be reused for the new sign
        }
    }
    for (int i = 0; i < strlen(new_message); i++) // For loop goes through each character of the new string and increments by 1 until the string is done
    {
        if ((new_message[i] >= 'A') && (new_message[i] <= 'Z')) // If the characters in the new message are uppercase and in the range of the alphabet
        {
            frequency_array[new_message[i] - 'A']++; // increasing the frequency array in the new message by 1 because the frequency of these letters represent how many of each letter needs to be bought for the new sign
        }
    }
    for (int i = 0; i < 26; i++) // For loop goes through each letter of the alphabet and increments by 1 until i is greater than 26
    {
        if (frequency_array[i] > 0) // If the letters in the frequency array have a frequency greater than 0
        {
            buy_letters += frequency_array[i]; // The number of letters that need to be bought is equal to the total of letters that have a frequency greater than 0 in the frequency array
        }
    }
    printf("%d ", buy_letters); // Prints the number of letters that needs to be bought for the new sign
}
