/*This file will provide the Unicode UTF8 encoding from a provided code point. The functions below
determine the actual bytes that encode up a particular character using anywhere from 1 to 3 bytes (in this 
computer program). 
 */

#include "samples/prototypes.h"
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


/* This function takes the unsigned short code point as input and the 
character array of utf8 bytes as input. The function determines whether 1, 2, or 3
bytes are necessary to represent the code point in binary. The number of bytes needed
are returned out of this function. Additionally, the character array of utf8 bytes
will be modified to hold all the bytes necessary so we can later map this encoding to the actual
character.
 */
int to_utf8(unsigned short code_point, unsigned char utf8_bytes[]) {
    int num_bytes = 1;
    
    //One byte needed
    if (code_point < 0x0080){
        num_bytes = 1;
        unsigned char fill = code_point & ((1<<7)-1); //Accessing the last 7 bits of the code point
       
        utf8_bytes[0] = fill;
        
    }
    //Two bytes needed
    else if (code_point < 0x07ff){
        num_bytes = 2;
        unsigned short unit = 1;
        
       
        unsigned char fill_1 = code_point >> 6;
        
        unsigned char mask = 6 << 5;
        fill_1 = mask | fill_1;
        utf8_bytes[0] = fill_1;
        
        unsigned char fill_2 = (code_point & ((unit<<6)-1));
        unsigned char mask2 = 2 << 6;
        fill_2 = mask2 | fill_2;
        utf8_bytes[1] = fill_2;

        
    }
    //Three bytes needed
    else {
        num_bytes = 3;
        unsigned short unit = 1;
        
        unsigned char mask = 14 << 4;
        
        unsigned char fill_1 = (code_point >> 12);
        utf8_bytes[0] = mask | fill_1;

        unsigned char mask2 = 2 << 6;

        //Shifting the code_point to the right by six will give us the lasst 8 bits but we want to keep only the last 6 the same and the leading two bits as 0. This can be done by using the & operator with 0b00111111 = 63.
        unsigned char fill_2 = (code_point >> 6) & 63; 
        utf8_bytes[1] = mask2 | fill_2;

        unsigned char mask3 = 2 << 6;
        
        unsigned char fill_3 = (code_point & ((unit<<6)-1));
        utf8_bytes[2] = mask3 | fill_3;
        
        
    }
    

    return num_bytes;
}


/* This function takes in the unsigned short code point,the character array of utf8 bytes, and 
the integer value of the length of the char array (number of bytes needed) all as inputs. The function
prints out the UTF-8 hexadecimal representation of the code point and prints out the actual character the 
encoding maps to.
 */
void print_utf8(unsigned short code_point, unsigned char utf8_bytes[], int utf8_bytes_length) {
    int i;
    printf("U+%04X   UTF-8 Hex:", code_point);
    for (i = 0; i < utf8_bytes_length; i++) {
       printf(" %02x", utf8_bytes[i]);
    }
    for ( ; i < 4; i++) {
        printf("   ");
    }
    utf8_bytes[utf8_bytes_length] = '\0';
    printf("Character: %s  \n", utf8_bytes);  
}

/* This function takes the code point argument as a char *, along with the min and max
values of an unsigned short as inputs. The function outputs errors if the code point is invalid
or if it is not in the specific range of an unsigned short. If no errors persist, the code point 
is returned in the form of an unsigned short.
 */
unsigned short convert_arg(const char *str, unsigned short low, unsigned short high) {
    char *end;
    long n = strtol(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid code point '%s'", str);
    }
    if (n < low || n > high) {
        error(1, 0, "Code point %s not in range [%u, %u]", str, low, high);
    }
    return n;
}

/*This main function takes the arguments the user enters as input and determines if there are enough arguments passed.
If so, it creates space in memory to store the utf8 bytes and calls the functions above to determine the actual encoding
and print it out to the user.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please supply one or more unicode code points in decimal or hex.");
    }
    
    for (int i = 1; i < argc; i++) {
        unsigned short code_point = convert_arg(argv[i], 0, USHRT_MAX);
        unsigned char utf8_bytes[4];
        int num_utf8_bytes = to_utf8(code_point, utf8_bytes);
        print_utf8(code_point, utf8_bytes, num_utf8_bytes);  
    }
    return 0;
}
