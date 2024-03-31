/*

WORKING :

     This assembler reads one line instructions at a time from the test.asm file then scan it and write its equivalent hex code into test.o
  file. The assembler scan the Assembly code for two times. In first scanning, It detects the label name & addresses, then save them. In the
  second scanning, it detects the opcode, data/address and get the addresses of the labels by checking the label names with the saved label
  names (saved label names are saved during first scanning). During second scanning, it also write the hex code into the test.o file. When an
  error occurs, then the compilations are terminated and delete all the generated hex code from the test.o file. If there are not any error in
  the assembly code, then it generates the equivalent hex code and print a messege as "Machine Code Generation - [Successful]".

FEATURES OF THIS ASSEMBLER :

1. All instructions are available in this assembler
2. It detects the level when conditional statement are take place, Ex - JMP Loop, CALL Label etc.
3. The facility to write comment, Ex - MVI A, 32H  ; This is the comment
4. Data and addresses are not case sensitive but instructions are case sensitive (Instructions should be in Upper Case letter)
5. If errors present in the assembly code, then it shows in which line the error present
6. when compilation is sucessful, then it shows "Machine Code Generation - [Successful]"

DRAWBACKS OF THIS ASSEMBLER :

1. It only works on Hexadecimal data/address. It does not supports Decimal or Octal or Binary data/address
2. The instructions are case sensitive (upper case letter) although the address/data are not case sensitive

*/


#include<stdio.h>
#include<stdlib.h>
#include <string.h>


#define start_line_number 1;
#define start_memory_address 0x1000;
#define max_store_labels 10


char *ptr0, *ptr1, *ptr2, *ptr3, *ptr4;               // These pointers are used to detect the whole Mnemonics in a single condition instead of checking characters one by one
unsigned char opcode;                                 // For storing the opcode
unsigned char data[5]={0};                            // To store the data as string
unsigned char low_byte_address[5]={0};                // To store the low byte address as string
unsigned char high_byte_address[5]={0};               // To store the high byte address as string
unsigned char instruction_size = 1;                   // Used for the instruction size i.e., 1 byte or 2 byte or 3 byte
unsigned char error_flag = 0;                         // For detecting the error. when error occurs the error_flag becomes 1 otherwise 0
unsigned char comment_flag = 0;                       // For detecting comment in a line
char level_name[max_store_labels][20] = {0};          // To store the level names as string
unsigned short level_address[max_store_labels] = {0}; // To store the level addresses
char level_number = 0;                                // Number of the level for link the lavel name and lavel addresses
unsigned short memory_address;                        // To store the memory address and is increamented by one when one byte hex code was written into the test.o file
unsigned short line_number;                           // To store the line number and is increamented by one when one line of the assembly code was read. It also used to detect the line number when an error occurs






/* The error function to print the error into the output terminal */
void error(){
  printf("\n\n----------------  ");
  printf("Something is wrong in the line number: %d", line_number);
  printf("  ----------------\n\n");
  instruction_size = 0;
  error_flag = 1;
}



/* This function detect the comment and and return 1 if there is comment */
char detect_comment(char *ptr){
  if(*ptr == ';') return 1;
  else return 0;
}



/* This function detect any other character or garbage present after the instruction */
void detect_garbage(char *end_ptr){
  end_ptr++;
  while(*end_ptr != '\n'){                     // loop until the pointer goes to the last character of the line
    if(*end_ptr == ' ') end_ptr++;             // if space ( ) present, then go to next pointer
    else if(detect_comment(end_ptr)) break;    // if semicolon (;) there, then treate as comment and out from the loop
    else if(*end_ptr == 0) break;              // if last character of the file, i.e., - \0 , then out from the loop
    else { error(); break;} 
  }
}



/* This function detect the scanned hex number is valid (i.e., - 0 to F) or not, and return 1 if the number is valid */
char valid_number(char *ptr){
  if(((*ptr >= '0') && (*ptr <= '9')) || ((*ptr >= 'A') && (*ptr <= 'F')) || ((*ptr >= 'a') && (*ptr <= 'f'))) return 1;
  else return 0;
}



/* This function convert a decimal number to hexadecimal number as a character for writing to the test.o file */
char convert_decimal_to_hex(char nibble){
  if(nibble <= 9) nibble += '0';            // for the character 0 to 9
  else nibble += '0' + 7;                   // for the character A to F
  return nibble;
}



/* This function divide the 16 bit address into four nibble and store them into four char variables as string */
void ready_the_address_to_print(short address){
  char nibble_0, nibble_1, nibble_2, nibble_3;

  /* converting the 16 bit address into 4 nibbles and store them in char variable */
  nibble_0 =  address & 0x000f;
  nibble_1 = (address & 0x00f0) >> 4;
  nibble_2 = (address & 0x0f00) >> 8;
  nibble_3 = address >> 12;

  /* construct the high byte address as string */
  high_byte_address[0] = convert_decimal_to_hex(nibble_3);
  high_byte_address[1] = convert_decimal_to_hex(nibble_2);
  high_byte_address[2] = '\n';
  
  /* construct the low byte address as string */
  low_byte_address[0] = convert_decimal_to_hex(nibble_1);
  low_byte_address[1] = convert_decimal_to_hex(nibble_0);
  low_byte_address[2] = '\n';
}



/* This function is used to detect the particular level when a call or jump is takes place in a particular level */
char *detect_level_name_n_address(char *level_scanned_ptr){
  
  char *end_ptr;
  char temp_string[20] = {0};  
  short address; 
  char *level_stored_ptr; 
  char address_flag = 0, i=0;
  
  /* this loop is for the copying level name into temporary string and detect the last character address */
  while(1){
    if((*level_scanned_ptr == 0) || (*level_scanned_ptr == '\n') || (*level_scanned_ptr == ' ')) break;
    temp_string[i++] = *level_scanned_ptr;
    level_scanned_ptr++;
  }
  end_ptr = --level_scanned_ptr;

  /* the below loop is for the detection of the particular level name and number, it scan all the stored level name until the matching */
  for(level_number=0; level_number<max_store_labels; level_number++){

    level_stored_ptr = level_name[level_number];
    level_scanned_ptr = temp_string;

    if((strcmp(level_stored_ptr, level_scanned_ptr)) == 0){ 
      address = level_address[level_number]; 
      ready_the_address_to_print(address); 
      instruction_size = 3;
      address_flag = 1;
      break;
      }
      else{
        address_flag = 0;
      }
  }
  if(!address_flag) error();        // if the scanned level is not matched, then error occurs
  return end_ptr;
}



/* for detect the data from the instruction */
char *detect_data(char *ptr){

  char *end_ptr;
  char *copy_ptr = ptr;
  instruction_size = 0;

  while(*ptr != 0){
  if((*ptr == 'H') || (*ptr == 'h')) break;
  ptr++;
  }

  end_ptr = ptr;
  ptr = ptr-2;
  if(ptr != copy_ptr) error();  // for detecting the garbage between instruction and the data

  if(valid_number(ptr)){
    data[0] = *ptr;
    ptr++;

    if(valid_number(ptr)){
      data[1] = *ptr;
      data[2] = '\n';
      instruction_size = 2;
      }
    else error();
  }
  else error();

  return end_ptr;
}



/* This function is for the detection of the scanned address from the instruction */
char *detect_scan_address(char *ptr){

  char *end_ptr;
  char *copy_ptr = ptr;

  while(*ptr != 0){
    if((*ptr == 'H') || (*ptr == 'h')) break;
    ptr++;
  }

  end_ptr = ptr;   // copy the last character location of the instruction
  ptr = ptr-4;

  if(ptr != copy_ptr) error();  // for detecting the garbage between instruction and the address

  if(valid_number(ptr)){
    high_byte_address[0] = *ptr;
    ptr++;
    
    if(valid_number(ptr)){
      high_byte_address[1] = *ptr;
      high_byte_address[2] = '\n';
      ptr++;
      
      if(valid_number(ptr)){
        low_byte_address[0] = *ptr;
        ptr++;

        if(valid_number(ptr)){
          low_byte_address[1] = *ptr;
          low_byte_address[2] = '\n';
          instruction_size = 3;
        }
        else error();
      }
      else error();
    }
    else error();
  }
  else error();

  return end_ptr;
}



/* this function is for skip the spaces between two charaters */
char *skip_space(char *ptr){
  while(*ptr == ' ') ptr++;
  return ptr;
}


/* This function detects the comma present in the instruction */
char detect_comma(char *ptr){
  char spaces = 0;
  ptr++;
  while(*ptr == ' ') {ptr++; spaces++;}

  if(*ptr == ',') return (spaces + 2); // space + 2 for pointing the next of the comma(,)
  else{ error(); return 0; }
}



/* This function is used to generate four consecutive pointers for detection of Mnemonics in a single time */
void update_ptr(char *ptr){
  ptr0 = ptr;
  ptr1 = ptr0 + 1;
  ptr2 = ptr1 + 1;
  ptr3 = ptr2 + 1;
  ptr4 = ptr3 + 1;
}



/* This function is used to save the level name and the address from the instruction */
char *save_level_address_n_name(char *ptr, char *colon_ptr){

  unsigned char i=0;
  skip_space(ptr);

  while(ptr != colon_ptr){ level_name[level_number][i++] = *ptr++; }       // copy the name of the level
  level_address[level_number] = memory_address;                            // copy the address of the level
  level_number++;
  colon_ptr++;
  colon_ptr = skip_space(colon_ptr);
  return colon_ptr;
}



/* This function detect the colon present in the instruction */
char *detect_colon(char *ptr){

  ptr = skip_space(ptr);
  while(*ptr != 0){
  if(*ptr == ':') break;
  ptr++;
  }
  return ptr;
}



/* A letter all Mnemonics */
void char_A(char *ptr){

  char *end_ptr;
  update_ptr(ptr);

  if((*ptr0 == 'A') && (*ptr1 == 'C') && (*ptr2 == 'I') && (*ptr3 == ' ')){        //////////// ACI data //////////////////
    opcode = 0xCE;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;   
  }
  else if((*ptr0 == 'A') && (*ptr1 == 'D') && (*ptr2 == 'I') && (*ptr3 == ' ')){   //////////// ADI data //////////////////
    opcode = 0xC6;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'A') && (*ptr1 == 'N') && (*ptr2 == 'I') && (*ptr3 == ' ')){   //////////// ANI data //////////////////
    opcode = 0xE6;
    ptr += 3;  
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'A') && (*ptr1 == 'D') && (*ptr2 == 'C') && (*ptr3 == ' ')){   //////////// ADC reg //////////////////
    ptr += 3;
    ptr = skip_space(ptr);

         if(*ptr == 'A') opcode = 0x8f;
    else if(*ptr == 'B') opcode = 0x88;
    else if(*ptr == 'C') opcode = 0x89;
    else if(*ptr == 'D') opcode = 0x8A;
    else if(*ptr == 'E') opcode = 0x8B;
    else if(*ptr == 'H') opcode = 0x8C;
    else if(*ptr == 'L') opcode = 0x8D;
    else if(*ptr == 'M') opcode = 0x8E;
    else error();
    end_ptr = ptr;
  }
  else if((*ptr0 == 'A') && (*ptr1 == 'D') && (*ptr2 == 'D') && (*ptr3 == ' ')){  //////////// ADD reg //////////////////
    ptr += 3;
    ptr = skip_space(ptr);

         if(*ptr == 'A') opcode = 0x87;
    else if(*ptr == 'B') opcode = 0x80;
    else if(*ptr == 'C') opcode = 0x81;
    else if(*ptr == 'D') opcode = 0x82;
    else if(*ptr == 'E') opcode = 0x83;
    else if(*ptr == 'H') opcode = 0x84;
    else if(*ptr == 'L') opcode = 0x85;
    else if(*ptr == 'M') opcode = 0x86;
    else error();
    end_ptr = ptr;
  }
  else if((*ptr0 == 'A') && (*ptr1 == 'N') && (*ptr2 == 'A') && (*ptr3 == ' ')){  //////////// ANA reg //////////////////
    ptr += 3;
    ptr = skip_space(ptr);

         if(*ptr == 'A') opcode = 0xA7;
    else if(*ptr == 'B') opcode = 0xA0;
    else if(*ptr == 'C') opcode = 0xA1;
    else if(*ptr == 'D') opcode = 0xA2;
    else if(*ptr == 'E') opcode = 0xA3;
    else if(*ptr == 'H') opcode = 0xA4;
    else if(*ptr == 'L') opcode = 0xA5;
    else if(*ptr == 'M') opcode = 0xA6;
    else error();
    end_ptr = ptr;
  }
  else error();
  detect_garbage(end_ptr);
}



/* C letter all Mnemonics */
void char_C(char *ptr){

  char *end_ptr;

  update_ptr(ptr);

  if((*ptr0 == 'C') && (*ptr1 == 'M') && (*ptr2 == 'A')){                           //////////// CMA //////////////////
   // printf("%s", ptr);
    ptr += 2;
    opcode = 0x2F;
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'M') && (*ptr2 == 'C')){                     //////////// CMC //////////////////
    ptr += 2;
    opcode = 0x3F;
    end_ptr = ptr; 
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'M') && (*ptr2 == 'P') && (*ptr3 == ' ')){  //////////// CMP reg ///////////////
    ptr += 3; 
    ptr = skip_space(ptr);
         if(*ptr == 'A') opcode = 0xBF;
    else if(*ptr == 'B') opcode = 0xB8;
    else if(*ptr == 'C') opcode = 0xB9;
    else if(*ptr == 'D') opcode = 0xBA;
    else if(*ptr == 'E') opcode = 0xBB;
    else if(*ptr == 'H') opcode = 0xBC;
    else if(*ptr == 'L') opcode = 0xBD;
    else if(*ptr == 'M') opcode = 0xBE;
    else error();
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'P') && (*ptr2 == 'I') && (*ptr3 == ' ')){                         //////////// CPI data //////////////
    opcode = 0xFE;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'A') && (*ptr2 == 'L') && (*ptr3 == 'L') && (*ptr4 == ' ')){     //////////// CALL label //////////////
    opcode = 0xCD;
    ptr += 4;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'C') && (*ptr2 == ' ')){                                        //////////// CC label ////////////////
    opcode = 0xDC;
    ptr += 2;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'M') && (*ptr2 == ' ')){                                       //////////// CM label /////////////////
    opcode = 0xFC;
    ptr += 2;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'N') && (*ptr2 == 'C') && (*ptr3 == ' ')){                    //////////// CNC label /////////////////
    opcode = 0xD4;
    ptr += 3;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'N') && (*ptr2 == 'Z') && (*ptr3 == ' ')){                    //////////// CNZ label /////////////////
    opcode = 0xC4;
    ptr += 3;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'P') && (*ptr2 == 'E') && (*ptr3 == ' ')){                   //////////// CPE label //////////////////
    opcode = 0xEC;
    ptr += 3;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'P') && (*ptr2 == 'O') && (*ptr3 == ' ')){                   //////////// CPO label //////////////////
    opcode = 0xE4;
    ptr += 3;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'P') && (*ptr2 == ' ')){                                    //////////// CP label //////////////////
    opcode = 0xF4;
    ptr += 2;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'C') && (*ptr1 == 'Z') && (*ptr2 == ' ')){                                    //////////// CZ label //////////////////
    opcode = 0xCC;
    ptr += 2;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else error();
  detect_garbage(end_ptr);
}



/* D letter all Mnemonics */
void char_D(char *ptr){

  char *end_ptr;
  update_ptr(ptr);

  if((*ptr0 == 'D') && (*ptr1 == 'A') && (*ptr2 == 'A')){                               //////////// DAA ////////////////////
    opcode = 0x27;
    ptr += 2;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'D') && (*ptr1 == 'A') && (*ptr2 == 'D') && (*ptr3 == ' ')){       //////////// DAD rp //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == 'B') opcode = 0x09;
    else if(*ptr == 'D') opcode = 0x19;
    else if(*ptr == 'H') opcode = 0x29;
    else if(*ptr == 'S'){ ptr++; if(*ptr == 'P') opcode = 0x39;}
    else error();  
    end_ptr = ptr; 
  }
  else if((*ptr0 == 'D') && (*ptr1 == 'C') && (*ptr2 == 'R') && (*ptr3 == ' ')){     //////////// DCR reg //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == 'A') opcode = 0x3D;
    else if(*ptr == 'B') opcode = 0x05;
    else if(*ptr == 'C') opcode = 0x0D;
    else if(*ptr == 'D') opcode = 0x15;
    else if(*ptr == 'E') opcode = 0x1D;
    else if(*ptr == 'H') opcode = 0x25;
    else if(*ptr == 'L') opcode = 0x2D;
    else if(*ptr == 'M') opcode = 0x35;
    else error(); 
    end_ptr = ptr;  
  }
  else if((*ptr0 == 'D') && (*ptr1 == 'C') && (*ptr2 == 'X') && (*ptr3 == ' ')){     //////////// DCX rp //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == 'B') opcode = 0x0B;
    else if(*ptr == 'D') opcode = 0x1B;
    else if(*ptr == 'H') opcode = 0x2B;
    else if(*ptr == 'S'){ ptr++; if(*ptr == 'P') opcode = 0x3B;}
    else error(); 
    end_ptr = ptr;  
  }
  else if((*ptr0 == 'D') && (*ptr1 == 'I')){                                       ////////////// DAA ///////////////////
    opcode = 0xF3;
    ptr ++;  
    end_ptr = ptr;
  }
  else error();
  detect_garbage(end_ptr);
}



/* E letter all Mnemonics */
void char_E(char *ptr){
  char *end_ptr; update_ptr(ptr);
  if((*ptr0 == 'E') && (*ptr1 == 'I')) opcode = 0xFB;
  end_ptr = ptr1;
  detect_garbage(end_ptr);
}



/* H letter all Mnemonics */
void char_H(char *ptr){
  char *end_ptr; update_ptr(ptr);
  if((*ptr0 == 'H') && (*ptr1 == 'L') && (*ptr2 == 'T')) opcode = 0x76;
  end_ptr = ptr2;
  detect_garbage(end_ptr);
}



/* I letter all Mnemonics */
void char_I(char *ptr){

  char *end_ptr;
  update_ptr(ptr);

  if((*ptr0 == 'I') && (*ptr1 == 'N') && (*ptr2 == 'R') && (*ptr3 == ' ')){           //////////// INR reg //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == 'A') opcode = 0x3C;
    else if(*ptr == 'B') opcode = 0x04;
    else if(*ptr == 'C') opcode = 0x0C;
    else if(*ptr == 'D') opcode = 0x14;
    else if(*ptr == 'E') opcode = 0x1C;
    else if(*ptr == 'H') opcode = 0x24;
    else if(*ptr == 'L') opcode = 0x2C;
    else if(*ptr == 'M') opcode = 0x34;
    else error(); 
    end_ptr = ptr; 
  }
  else if((*ptr0 == 'I') && (*ptr1 == 'N') && (*ptr2 == 'X') && (*ptr3 == ' ')){     //////////// INX rp //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == 'B') opcode = 0x03;
    else if(*ptr == 'D') opcode = 0x13;
    else if(*ptr == 'H') opcode = 0x23;
    else if(*ptr == 'S'){ ptr++; if(*ptr == 'P') opcode = 0x33;}
    else error(); 
    end_ptr = ptr;
  }
  else if((*ptr0 == 'I') && (*ptr1 == 'N') && (*ptr2 == ' ')){                     ///////// IN port_address //////////////
    opcode = 0xDB;
    ptr += 2;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;
  }
  else error();
  detect_garbage(end_ptr);
}



/* J letter all Mnemonics */
void char_J(char *ptr){

  char *end_ptr;
  update_ptr(ptr);

  if((*ptr0 == 'J') && (*ptr1 == 'C') && (*ptr2 == ' ')){                            //////////// JC label //////////////////
    opcode = 0xDA;
    ptr += 2;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'J') && (*ptr1 == 'M') && (*ptr2 == 'P') && (*ptr3 == ' ')){     //////////// JMP label //////////////////
    opcode = 0xC3;
    ptr += 3;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'J') && (*ptr1 == 'M') && (*ptr2 == ' ')){                       //////////// JM label //////////////////
    opcode = 0xFA;
    ptr += 2;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'J') && (*ptr1 == 'N') && (*ptr2 == 'C') && (*ptr3 == ' ')){    //////////// JNC label //////////////////
    opcode = 0xD2;
    ptr += 3;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'J') && (*ptr1 == 'N') && (*ptr2 == 'Z') && (*ptr3 == ' ')){   //////////// JNZ label //////////////////
    opcode = 0xC2;
    ptr += 3;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'J') && (*ptr1 == 'P') && (*ptr2 == 'E') && (*ptr3 == ' ')){   //////////// JPE label //////////////////
    opcode = 0xEA;
    ptr += 3;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'J') && (*ptr1 == 'P') && (*ptr2 == 'O') && (*ptr3 == ' ')){   //////////// JPO label //////////////////
    opcode = 0xE2;
    ptr += 3;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'J') && (*ptr1 == 'P') && (*ptr2 == ' ')){                     //////////// JP label //////////////////
    opcode = 0xF2;
    ptr += 2;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'J') && (*ptr1 == 'Z') && (*ptr2 == ' ')){                     //////////// JP label //////////////////
    opcode = 0xCA;
    ptr += 2;
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_level_name_n_address(ptr);
    end_ptr = ptr;
  }
  else error();
  detect_garbage(end_ptr);
}




/* L letter all Mnemonics */
void char_L(char *ptr){

  char *end_ptr;
  char comma_spaces = 0;
  update_ptr(ptr);

  if((*ptr0 == 'L') && (*ptr1 == 'D') && (*ptr2 == 'A') && (*ptr3 == ' ')){                            //////////// LDA address //////////////////
    opcode = 0x3A;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_scan_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'L') && (*ptr1 == 'H') && (*ptr2 == 'L') && (*ptr3 == 'D') && (*ptr4 == ' ')){     //////////// LHLD address //////////////////
    opcode = 0x2A;
    ptr += 4; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_scan_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'L') && (*ptr1 == 'X') && (*ptr2 == 'I') && (*ptr3 == ' ')){                       //////////// LXI address //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    update_ptr(ptr);

         if(*ptr0 == 'B') opcode = 0x01;
    else if(*ptr0 == 'D') opcode = 0x11;
    else if(*ptr0 == 'H') opcode = 0x21;
    else if((*ptr0 == 'S') && (*ptr1 == 'P')){ opcode = 0x31; ptr++;}
    else error();

    comma_spaces = detect_comma(ptr);
    if(comma_spaces){
    ptr += comma_spaces;
    ptr = skip_space(ptr);
    ptr = detect_scan_address(ptr);
   }
   end_ptr = ptr;
  }
  else if((*ptr0 == 'L') && (*ptr1 == 'D') && (*ptr2 == 'A') && (*ptr3 == 'X') && (*ptr4 == ' ')){      //////////// LDAX rp //////////////////
    ptr += 4; 
    ptr = skip_space(ptr);

         if(*ptr == 'B') opcode = 0x0A;
    else if(*ptr == 'D') opcode = 0x1A;
    else error(); 
    end_ptr = ptr; 
  }
  else error();
  detect_garbage(end_ptr);
}



/* M letter all Mnemonics */
void char_M(char *ptr){

  char *end_ptr;
  char comma_spaces = 0;
  update_ptr(ptr);

  if((*ptr0 == 'M') && (*ptr1 == 'V') && (*ptr2 == 'I') && (*ptr3 == ' ')){         //////////// MVI reg, data //////////////////
    ptr += 3;  // Going to MVI next
    ptr = skip_space(ptr);    // removing the spaces

        if(*ptr == 'A') opcode = 0x3E;
   else if(*ptr == 'B') opcode = 0x06;
   else if(*ptr == 'C') opcode = 0x0E;
   else if(*ptr == 'D') opcode = 0x16;
   else if(*ptr == 'E') opcode = 0x1E;
   else if(*ptr == 'H') opcode = 0x26;
   else if(*ptr == 'L') opcode = 0x2E;
   else if(*ptr == 'M') opcode = 0x36;

   comma_spaces = detect_comma(ptr);
   if(comma_spaces){
    ptr += comma_spaces;
    ptr = skip_space(ptr);
    ptr = detect_data(ptr);
    end_ptr = ptr;
   } 
  }

  else if((*ptr0 == 'M') && (*ptr1 == 'O') && (*ptr2 == 'V') && (*ptr3 == ' ')){     //////////// MOV A, reg //////////////////
    instruction_size = 1;
    ptr = ptr + 3;  // Going to MVI next
    ptr = skip_space(ptr);    // removing the spaces

         if(*ptr == 'A'){
          comma_spaces = detect_comma(ptr);
          if(comma_spaces){
          ptr += comma_spaces;
          ptr = skip_space(ptr);
               if(*ptr == 'A') opcode = 0x7F;
          else if(*ptr == 'B') opcode = 0x78;
          else if(*ptr == 'C') opcode = 0x79;
          else if(*ptr == 'D') opcode = 0x7A;
          else if(*ptr == 'E') opcode = 0x7B;
          else if(*ptr == 'H') opcode = 0x7C;
          else if(*ptr == 'L') opcode = 0x7D;
          else if(*ptr == 'M') opcode = 0x7E;
          else error();
          end_ptr = ptr;
          }
         }
    else if(*ptr == 'B'){
          comma_spaces = detect_comma(ptr);
          if(comma_spaces){
          ptr += comma_spaces;
          ptr = skip_space(ptr);
               if(*ptr == 'A') opcode = 0x47;
          else if(*ptr == 'B') opcode = 0x40;
          else if(*ptr == 'C') opcode = 0x41;
          else if(*ptr == 'D') opcode = 0x42;
          else if(*ptr == 'E') opcode = 0x43;
          else if(*ptr == 'H') opcode = 0x44;
          else if(*ptr == 'L') opcode = 0x45;
          else if(*ptr == 'M') opcode = 0x46;
          else error();
          end_ptr = ptr;
          }
    }
    else if(*ptr == 'C'){
          comma_spaces = detect_comma(ptr);
          if(comma_spaces){
          ptr += comma_spaces;
          ptr = skip_space(ptr);
               if(*ptr == 'A') opcode = 0x4F;
          else if(*ptr == 'B') opcode = 0x48;
          else if(*ptr == 'C') opcode = 0x49;
          else if(*ptr == 'D') opcode = 0x4A;
          else if(*ptr == 'E') opcode = 0x4B;
          else if(*ptr == 'H') opcode = 0x4C;
          else if(*ptr == 'L') opcode = 0x4D;
          else if(*ptr == 'M') opcode = 0x4E;
          else error();
          end_ptr = ptr;
          }
    }
    else if(*ptr == 'D'){
          comma_spaces = detect_comma(ptr);
          if(comma_spaces){
          ptr += comma_spaces;
          ptr = skip_space(ptr);
               if(*ptr == 'A') opcode = 0x57;
          else if(*ptr == 'B') opcode = 0x50;
          else if(*ptr == 'C') opcode = 0x51;
          else if(*ptr == 'D') opcode = 0x52;
          else if(*ptr == 'E') opcode = 0x53;
          else if(*ptr == 'H') opcode = 0x54;
          else if(*ptr == 'L') opcode = 0x55;
          else if(*ptr == 'M') opcode = 0x56;
          else error();
          end_ptr = ptr;
          }
    }
    else if(*ptr == 'E'){
          comma_spaces = detect_comma(ptr);
          if(comma_spaces){
          ptr += comma_spaces;
          ptr = skip_space(ptr);
               if(*ptr == 'A') opcode = 0x5F;
          else if(*ptr == 'B') opcode = 0x58;
          else if(*ptr == 'C') opcode = 0x59;
          else if(*ptr == 'D') opcode = 0x5A;
          else if(*ptr == 'E') opcode = 0x5B;
          else if(*ptr == 'H') opcode = 0x5C;
          else if(*ptr == 'L') opcode = 0x5D;
          else if(*ptr == 'M') opcode = 0x5E;
          else error();
          end_ptr = ptr;
          }
    }
    else if(*ptr == 'H'){
          comma_spaces = detect_comma(ptr);
          if(comma_spaces){
          ptr += comma_spaces;
          ptr = skip_space(ptr);
               if(*ptr == 'A') opcode = 0x67;
          else if(*ptr == 'B') opcode = 0x60;
          else if(*ptr == 'C') opcode = 0x61;
          else if(*ptr == 'D') opcode = 0x62;
          else if(*ptr == 'E') opcode = 0x63;
          else if(*ptr == 'H') opcode = 0x64;
          else if(*ptr == 'L') opcode = 0x65;
          else if(*ptr == 'M') opcode = 0x66;
          else error();
          end_ptr = ptr;
          }
    }
    else if(*ptr == 'L'){
          comma_spaces = detect_comma(ptr);
          if(comma_spaces){
          ptr += comma_spaces;
          ptr = skip_space(ptr);
               if(*ptr == 'A') opcode = 0x6F;
          else if(*ptr == 'B') opcode = 0x68;
          else if(*ptr == 'C') opcode = 0x69;
          else if(*ptr == 'D') opcode = 0x6A;
          else if(*ptr == 'E') opcode = 0x6B;
          else if(*ptr == 'H') opcode = 0x6C;
          else if(*ptr == 'L') opcode = 0x6D;
          else if(*ptr == 'M') opcode = 0x6E;
          else error();
          end_ptr = ptr;
          }
    }
    else if(*ptr == 'M'){
          comma_spaces = detect_comma(ptr);
          if(comma_spaces){
          ptr += comma_spaces;
          ptr = skip_space(ptr);
               if(*ptr == 'A') opcode = 0x77;
          else if(*ptr == 'B') opcode = 0x70;
          else if(*ptr == 'C') opcode = 0x71;
          else if(*ptr == 'D') opcode = 0x72;
          else if(*ptr == 'E') opcode = 0x73;
          else if(*ptr == 'H') opcode = 0x74;
          else if(*ptr == 'L') opcode = 0x75;
          else error();
          end_ptr = ptr;
          }
    }
  }
  else{
    instruction_size = 0;
    error();
  }
  detect_garbage(end_ptr);
}



/* N letter all Mnemonics */
void char_N(char *ptr){
  char *end_ptr; update_ptr(ptr);
  if((*ptr0 == 'N') && (*ptr1 == 'O') && (*ptr2 == 'P')) opcode = 0x00;
  end_ptr = ptr2;
  detect_garbage(end_ptr);
}



/* O letter all Mnemonics */
void char_O(char *ptr){

  char *end_ptr;
  update_ptr(ptr);

  if((*ptr0 == 'O') && (*ptr1 == 'R') && (*ptr2 == 'A') && (*ptr3 == ' ')){           //////////// ORA reg //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == 'A') opcode = 0xB7;
    else if(*ptr == 'B') opcode = 0xB0;
    else if(*ptr == 'C') opcode = 0xB1;
    else if(*ptr == 'D') opcode = 0xB2;
    else if(*ptr == 'E') opcode = 0xB3;
    else if(*ptr == 'H') opcode = 0xB4;
    else if(*ptr == 'L') opcode = 0xB5;
    else if(*ptr == 'M') opcode = 0xB6;
    else error(); 
    end_ptr = ptr;
  }
  else if((*ptr0 == 'O') && (*ptr1 == 'R') && (*ptr2 == 'I') && (*ptr3 == ' ')){     //////////// ORI data //////////////////
    opcode = 0xF6;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'O') && (*ptr1 == 'U') && (*ptr2 == 'T') && (*ptr3 == ' ')){     //////// OUT port_address //////////////
    opcode = 0xD3;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;
  }
  else error();
  detect_garbage(end_ptr);
}



/* P letter all Mnemonics */
void char_P(char *ptr){

  char *end_ptr;
  update_ptr(ptr);

  if((*ptr0 == 'P') && (*ptr1 == 'C') && (*ptr2 == 'H') && (*ptr3 == 'L')){                             //////////// PCHL ///////////////////
    opcode = 0xE9;
    ptr += 3;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'P') && (*ptr1 == 'O') && (*ptr2 == 'P') && (*ptr3 == ' ')){                       //////////// POP rp //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);
    update_ptr(ptr);

         if(*ptr == 'B') opcode = 0xC1;
    else if(*ptr == 'D') opcode = 0xD1;
    else if(*ptr == 'H') opcode = 0xE1;
    else if((*ptr0 == 'P') && (*ptr1 == 'S') && (*ptr2 == 'W')){ opcode = 0xF1; ptr += 2;}
    else error(); 
    end_ptr = ptr;  
  }
  else if((*ptr0 == 'P') && (*ptr1 == 'U') && (*ptr2 == 'S') && (*ptr3 == 'H') && (*ptr4 == ' ')){     //////////// PUSH rp ////////////////
    ptr += 4; 
    ptr = skip_space(ptr);
    update_ptr(ptr);

         if(*ptr == 'B') opcode = 0xC5;
    else if(*ptr == 'D') opcode = 0xD5;
    else if(*ptr == 'H') opcode = 0xE5;
    else if((*ptr0 == 'P') && (*ptr1 == 'S') && (*ptr2 == 'W')){ opcode = 0xF5; ptr += 2;}
    else error(); 
    end_ptr = ptr;  
  }
  else error();
  detect_garbage(end_ptr);
}



/* R letter all Mnemonics */
void char_R(char *ptr){

  char *end_ptr;
  update_ptr(ptr);

  if((*ptr0 == 'R') && (*ptr1 == 'A') && (*ptr2 == 'L')){           //////////// RAL //////////////////
    opcode = 0x17;
    ptr += 2;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'A') && (*ptr2 == 'R')){      //////////// RAR //////////////////
    opcode = 0x1F;
    ptr += 2;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'C')){                        //////////// RC //////////////////
    opcode = 0xD8;
    ptr ++; 
    end_ptr = ptr; 
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'E') && (*ptr2 == 'T')){     //////////// RET //////////////////
    opcode = 0xC9;
    ptr += 2; 
    end_ptr = ptr; 
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'I') && (*ptr2 == 'M')){     //////////// RIM //////////////////
    opcode = 0x20;
    ptr += 2;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'L') && (*ptr2 == 'C')){     //////////// RLC //////////////////
    opcode = 0x07;
    ptr += 2; 
    end_ptr = ptr; 
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'M')){                       //////////// RM //////////////////
    opcode = 0xF8;
    ptr ++;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'N') && (*ptr2 == 'C')){    //////////// RNC //////////////////
    opcode = 0xD0;
    ptr += 2;
    end_ptr = ptr;  
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'N') && (*ptr2 == 'Z')){    //////////// RNZ //////////////////
    opcode = 0xC0;
    ptr += 2;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'P') && (*ptr2 == 'E')){     //////////// RPE //////////////////
    opcode = 0xE8;
    ptr += 2;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'P') && (*ptr2 == 'O')){     //////////// RPO //////////////////
    opcode = 0xE0;
    ptr += 2;  
    end_ptr = ptr;
  }
    else if((*ptr0 == 'R') && (*ptr1 == 'P')){                      //////////// RP //////////////////
    opcode = 0xF0;
    ptr ++;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'R') && (*ptr2 == 'C')){     //////////// RRC //////////////////
    opcode = 0x0F;
    ptr += 2;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'S') && (*ptr2 == 'T') && (*ptr3 == ' ')){ ////// RTS no. //////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == '0') opcode = 0xC7;
    else if(*ptr == '1') opcode = 0xCF;
    else if(*ptr == '2') opcode = 0xD7;
    else if(*ptr == '3') opcode = 0xDF;
    else if(*ptr == '4') opcode = 0xE7;
    else if(*ptr == '5') opcode = 0xEF;
    else if(*ptr == '6') opcode = 0xF7;
    else if(*ptr == '7') opcode = 0xFF;
    else error();   
    end_ptr = ptr;
  }
  else if((*ptr0 == 'R') && (*ptr1 == 'Z')){                        //////////// RZ //////////////////
    opcode = 0xC8;
    ptr ++;  
    end_ptr = ptr;
  }
  else error();
  detect_garbage(end_ptr);
}



/* S letter all Mnemonics */
void char_S(char *ptr){

  char *end_ptr;
  update_ptr(ptr);

  if((*ptr0 == 'S') && (*ptr1 == 'B') && (*ptr2 == 'B') && (*ptr3 == ' ')){          //////////// SBB reg //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == 'A') opcode = 0x9F;
    else if(*ptr == 'B') opcode = 0x98;
    else if(*ptr == 'C') opcode = 0x99;
    else if(*ptr == 'D') opcode = 0x9A;
    else if(*ptr == 'E') opcode = 0x9B;
    else if(*ptr == 'H') opcode = 0x9C;
    else if(*ptr == 'L') opcode = 0x9D;
    else if(*ptr == 'M') opcode = 0x9E;
    else error();
    end_ptr = ptr;   
  }
  else if((*ptr0 == 'S') && (*ptr1 == 'B') && (*ptr2 == 'I') && (*ptr3 == ' ')){     //////////// SBI data //////////////////
    opcode = 0xDE;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;
  }
   else if((*ptr0 == 'S') && (*ptr1 == 'I') && (*ptr2 == 'M')){                      ////////////// SIM ////////////////////
    opcode = 0x30;
    ptr += 2;
    end_ptr = ptr;  
  }
   else if((*ptr0 == 'S') && (*ptr1 == 'P') && (*ptr2 == 'H') && (*ptr3 == 'L')){    ///////////// SPHL ///////////////////
    opcode = 0xF9;
    ptr += 3;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'S') && (*ptr1 == 'T') && (*ptr2 == 'A') && (*ptr3 == 'X') && (*ptr4 == ' ')){  ///// STAX rp /////////
    ptr += 4; 
    ptr = skip_space(ptr);

         if(*ptr == 'B') opcode = 0x02;
    else if(*ptr == 'D') opcode = 0x12;
    else error();  
    end_ptr = ptr; 
  }
  else if((*ptr0 == 'S') && (*ptr1 == 'T') && (*ptr2 == 'C')){                          //////////// STC //////////////////
    opcode = 0x37;
    ptr += 2;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'S') && (*ptr1 == 'U') && (*ptr2 == 'B') && (*ptr3 == ' ')){    //////////// SUB reg //////////////////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == 'A') opcode = 0x97;
    else if(*ptr == 'B') opcode = 0x90;
    else if(*ptr == 'C') opcode = 0x91;
    else if(*ptr == 'D') opcode = 0x92;
    else if(*ptr == 'E') opcode = 0x93;
    else if(*ptr == 'H') opcode = 0x94;
    else if(*ptr == 'L') opcode = 0x95;
    else if(*ptr == 'M') opcode = 0x96;
    else error(); 
    end_ptr = ptr;  
  }
  else if((*ptr0 == 'S') && (*ptr1 == 'U') && (*ptr2 == 'I') && (*ptr3 == ' ')){                       ////////// SUI data ////////////////
    opcode = 0xD6;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'S') && (*ptr1 == 'H') && (*ptr2 == 'L') && (*ptr3 == 'D') && (*ptr4 == ' ')){     //////////// SHLD //////////////////
    opcode = 0x22;
    ptr += 4; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_scan_address(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'S') && (*ptr1 == 'T') && (*ptr2 == 'A') && (*ptr3 == ' ')){                       //////////// STA //////////////////
    opcode = 0x32;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_scan_address(ptr);
    end_ptr = ptr;
  }
  else error();
  detect_garbage(end_ptr);
}



/* X letter all Mnemonics */
void char_X(char *ptr){

  char *end_ptr;
  update_ptr(ptr);

  if((*ptr0 == 'X') && (*ptr1 == 'C') && (*ptr2 == 'H') && (*ptr3 == 'G')){            //////////// XCHG //////////////////
    opcode = 0xEB;
    ptr += 3;  
    end_ptr = ptr;
  }
  else if((*ptr0 == 'X') && (*ptr1 == 'R') && (*ptr2 == 'A') && (*ptr3 == ' ')){      /////////// XRA reg /////////////////
    ptr += 3; 
    ptr = skip_space(ptr);

         if(*ptr == 'A') opcode = 0xAF;
    else if(*ptr == 'B') opcode = 0xA8;
    else if(*ptr == 'C') opcode = 0xA9;
    else if(*ptr == 'D') opcode = 0xAA;
    else if(*ptr == 'E') opcode = 0xAB;
    else if(*ptr == 'H') opcode = 0xAC;
    else if(*ptr == 'L') opcode = 0xAD;
    else if(*ptr == 'M') opcode = 0xAE;
    else error();  
    end_ptr = ptr; 
  }
  else if((*ptr0 == 'X') && (*ptr1 == 'R') && (*ptr2 == 'I') && (*ptr3 == ' ')){   //////////// XRI data //////////////////
    opcode = 0xEE;
    ptr += 3; 
    ptr = skip_space(ptr);    // removing the spaces
    ptr = detect_data(ptr);
    end_ptr = ptr;
  }
  else if((*ptr0 == 'X') && (*ptr1 == 'T') && (*ptr2 == 'H') && (*ptr3 == 'L')){     //////////// XTHL //////////////////
    opcode = 0xE3;
    ptr += 3;  
    end_ptr = ptr;
  }
  else error();
  detect_garbage(end_ptr);
}



/* This function is the first step for detecting instructions. It detects the first character of the Mnemonics and send the execution into the right letter function */
void direction_to_detect_instructions(char *ptr){

  ptr = skip_space(ptr);
  if(detect_comment(ptr)){ comment_flag = 1; goto skip;}    // detecting the line is comment or not
  char *colon_ptr = detect_colon(ptr);                      // detect whether there are level present or not
  if(*colon_ptr == ':') ptr = colon_ptr + 1;
  ptr = skip_space(ptr);

  char character = *ptr;

       if(character == 'A') char_A(ptr);
  else if(character == 'C') char_C(ptr);
  else if(character == 'D') char_D(ptr);
  else if(character == 'E') char_E(ptr);
  else if(character == 'H') char_H(ptr);
  else if(character == 'I') char_I(ptr);
  else if(character == 'J') char_J(ptr);
  else if(character == 'L') char_L(ptr);
  else if(character == 'M') char_M(ptr); 
  else if(character == 'N') char_N(ptr);
  else if(character == 'O') char_O(ptr);
  else if(character == 'P') char_P(ptr);
  else if(character == 'R') char_R(ptr);
  else if(character == 'S') char_S(ptr);
  else if(character == 'X') char_X(ptr);
  else error();

  skip:;
}



/* This function is used to detect the actual level address and then save it */
char detect_levels(char *ptr){

  char instruction_size = 0;

  ptr = skip_space(ptr);
  if(detect_comment(ptr)) goto skip;                                           // detecting the line is comment or not
  char *colon_ptr = detect_colon(ptr);                                         // detect whether there are level present or not
  if(*colon_ptr == ':')  ptr = save_level_address_n_name(ptr, colon_ptr);      // save the lavel address and name
  update_ptr(ptr);

       if(*ptr0 == 'A'){
             if((*ptr1 == 'C') && (*ptr2 == 'I') && (*ptr3 == ' ')) instruction_size = 2;
        else if((*ptr1 == 'D') && (*ptr2 == 'C') && (*ptr3 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'D') && (*ptr2 == 'D') && (*ptr3 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'D') && (*ptr2 == 'I') && (*ptr3 == ' ')) instruction_size = 2;
        else if((*ptr1 == 'N') && (*ptr2 == 'A') && (*ptr3 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'N') && (*ptr2 == 'I') && (*ptr3 == ' ')) instruction_size = 2;
        else error();
       }
  else if(*ptr0 == 'C'){
             if((*ptr1 == 'A') && (*ptr2 == 'L') && (*ptr3 == 'L') && (*ptr4 == ' ')) instruction_size = 3;
        else if((*ptr1 == 'C') && (*ptr2 == ' '))                                     instruction_size = 3;
        else if((*ptr1 == 'M') && (*ptr2 == 'A'))                                     instruction_size = 1;
        else if((*ptr1 == 'M') && (*ptr2 == 'C'))                                     instruction_size = 1;
        else if((*ptr1 == 'M') && (*ptr2 == 'P') && (*ptr3 == ' '))                   instruction_size = 1;
        else if((*ptr1 == 'M') && (*ptr2 == ' '))                                     instruction_size = 3;
        else if((*ptr1 == 'N') && (*ptr2 == 'C') && (*ptr3 == ' '))                   instruction_size = 3;
        else if((*ptr1 == 'N') && (*ptr2 == 'Z') && (*ptr3 == ' '))                   instruction_size = 3;
        else if((*ptr1 == 'P') && (*ptr2 == 'E') && (*ptr3 == ' '))                   instruction_size = 3;
        else if((*ptr1 == 'P') && (*ptr2 == 'I') && (*ptr3 == ' '))                   instruction_size = 2;
        else if((*ptr1 == 'P') && (*ptr2 == 'O') && (*ptr3 == ' '))                   instruction_size = 3;
        else if((*ptr1 == 'P') && (*ptr2 == ' '))                                     instruction_size = 3;
        else if((*ptr1 == 'Z') && (*ptr2 == ' '))                                     instruction_size = 3;
        else error();
  }
  else if(*ptr0 == 'D'){
             if((*ptr1 == 'A') && (*ptr2 == 'A'))                   instruction_size = 1;
        else if((*ptr1 == 'A') && (*ptr2 == 'D') && (*ptr3 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'C') && (*ptr2 == 'R') && (*ptr3 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'C') && (*ptr2 == 'X') && (*ptr3 == ' ')) instruction_size = 1;
        else if(*ptr1 == 'I')                                       instruction_size = 1;
        else error();
  }
  else if((*ptr0 == 'E') && (*ptr1 == 'I'))                         instruction_size = 1;
  else if((*ptr0 == 'H') && (*ptr1 == 'L') && (*ptr2 == 'T'))       instruction_size = 1;

  else if(*ptr0 == 'I'){
             if((*ptr1 == 'N') && (*ptr2 == 'R') && (*ptr3 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'N') && (*ptr2 == 'X') && (*ptr3 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'N') && (*ptr2 == ' '))                    instruction_size = 2;
        else error();
  }
  else if(*ptr0 == 'J'){
             if((*ptr1 == 'C') && (*ptr2 == ' '))                   instruction_size = 3;
        else if((*ptr1 == 'M') && (*ptr2 == 'P') && (*ptr3 == ' ')) instruction_size = 3;
        else if((*ptr1 == 'M') && (*ptr2 == ' '))                   instruction_size = 3;
        else if((*ptr1 == 'N') && (*ptr2 == 'C') && (*ptr3 == ' ')) instruction_size = 3;
        else if((*ptr1 == 'N') && (*ptr2 == 'Z') && (*ptr3 == ' ')) instruction_size = 3;
        else if((*ptr1 == 'P') && (*ptr2 == 'E') && (*ptr3 == ' ')) instruction_size = 3;
        else if((*ptr1 == 'P') && (*ptr2 == 'O') && (*ptr3 == ' ')) instruction_size = 3;
        else if((*ptr1 == 'P') && (*ptr2 == ' '))                   instruction_size = 3;
        else if((*ptr1 == 'Z') && (*ptr2 == ' '))                   instruction_size = 3;
        else error();
  }
  else if(*ptr0 == 'L'){
             if((*ptr1 == 'D') && (*ptr2 == 'A') && (*ptr3 == 'X') && (*ptr4 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'D') && (*ptr2 == 'A') && (*ptr3 == ' '))                   instruction_size = 3;
        else if((*ptr1 == 'H') && (*ptr2 == 'L') && (*ptr3 == 'D') && (*ptr4 == ' ')) instruction_size = 3;
        else if((*ptr1 == 'X') && (*ptr2 == 'I') && (*ptr3 == ' '))                   instruction_size = 3;
        else error();
  }
  else if(*ptr0 == 'M'){
             if((*ptr1 == 'O') && (*ptr2 == 'V') && (*ptr3 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'V') && (*ptr2 == 'I') && (*ptr3 == ' ')) instruction_size = 2;
        else error();
  }
  else if((*ptr0 == 'N') && (*ptr1 == 'O') && (*ptr2 == 'P'))       instruction_size = 1;

  else if(*ptr0 == 'O'){
             if((*ptr1 == 'R') && (*ptr2 == 'A') && (*ptr3 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'R') && (*ptr2 == 'I') && (*ptr3 == ' ')) instruction_size = 2;
        else if((*ptr1 == 'U') && (*ptr2 == 'T') && (*ptr3 == ' ')) instruction_size = 2;
        else error();
  }
  else if(*ptr0 == 'P'){
             if((*ptr1 == 'C') && (*ptr2 == 'H') && (*ptr3 == 'L'))                   instruction_size = 1;
        else if((*ptr1 == 'O') && (*ptr2 == 'P') && (*ptr3 == ' '))                   instruction_size = 1;
        else if((*ptr1 == 'U') && (*ptr2 == 'S') && (*ptr3 == 'H') && (*ptr4 == ' ')) instruction_size = 1;
        else error();
  }
  else if(*ptr0 == 'R'){
             if((*ptr1 == 'A') && (*ptr2 == 'L'))     instruction_size = 1;
        else if((*ptr1 == 'A') && (*ptr2 == 'R'))     instruction_size = 1;
        else if (*ptr1 == 'C')                        instruction_size = 1;
        else if((*ptr1 == 'E') && (*ptr2 == 'T'))     instruction_size = 1;
        else if((*ptr1 == 'I') && (*ptr2 == 'M'))     instruction_size = 1;
        else if((*ptr1 == 'L') && (*ptr2 == 'C'))     instruction_size = 1;
        else if (*ptr1 == 'M')                        instruction_size = 1;
        else if((*ptr1 == 'N') && (*ptr2 == 'C'))     instruction_size = 1;
        else if((*ptr1 == 'N') && (*ptr2 == 'Z'))     instruction_size = 1;
        else if((*ptr1 == 'P') && (*ptr2 == 'E'))     instruction_size = 1;
        else if((*ptr1 == 'P') && (*ptr2 == 'O'))     instruction_size = 1;
        else if (*ptr1 == 'P')                        instruction_size = 1;
        else if((*ptr1 == 'R') && (*ptr2 == 'C'))     instruction_size = 1;
        else if((*ptr1 == 'S') && (*ptr2 == 'T') && (*ptr3 == ' ')) instruction_size = 1;
        else if (*ptr1 == 'Z')                         instruction_size = 1;
        else error();
  }
  else if(*ptr0 == 'S'){
             if((*ptr1 == 'B') && (*ptr2 == 'B') && (*ptr3 == ' '))                   instruction_size = 1;
        else if((*ptr1 == 'B') && (*ptr2 == 'I') && (*ptr3 == ' '))                   instruction_size = 2;
        else if((*ptr1 == 'H') && (*ptr2 == 'L') && (*ptr3 == 'D') && (*ptr4 == ' ')) instruction_size = 3;
        else if((*ptr1 == 'I') && (*ptr2 == 'M') && (*ptr3 == ' '))                   instruction_size = 1;
        else if((*ptr1 == 'P') && (*ptr2 == 'H') && (*ptr3 == 'L') && (*ptr4 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'T') && (*ptr2 == 'A') && (*ptr3 == 'X') && (*ptr4 == ' ')) instruction_size = 1;
        else if((*ptr1 == 'T') && (*ptr2 == 'A') && (*ptr3 == ' '))                   instruction_size = 3;
        else if((*ptr1 == 'T') && (*ptr2 == 'C'))                                     instruction_size = 1;
        else if((*ptr1 == 'U') && (*ptr2 == 'B') && (*ptr3 == ' '))                   instruction_size = 1;
        else if((*ptr1 == 'U') && (*ptr2 == 'I') && (*ptr3 == ' '))                   instruction_size = 2;
        else error();
  }
  else if(*ptr0 == 'X'){
             if((*ptr1 == 'C') && (*ptr2 == 'H') && (*ptr3 == 'G'))     instruction_size = 1;
        else if((*ptr1 == 'R') && (*ptr2 == 'A') && (*ptr3 == ' '))     instruction_size = 1;
        else if((*ptr1 == 'R') && (*ptr2 == 'I') && (*ptr3 == ' '))     instruction_size = 2;
        else if((*ptr1 == 'T') && (*ptr2 == 'H') && (*ptr3 == 'L'))     instruction_size = 1;
        else error();
  }
  else error();

skip:
  return instruction_size;
}



/* The main function */
int main() {
  char str[50];             // array for store one line at a time for detect the instruction
  char instruction_byte_size;    // used for store the instruction size when detect the level address
  FILE *read_fp, *write_fp;


//////////////* First scanning the code for detect the level addresses and the level names *////////////////

  read_fp = fopen("test.asm", "r");

  line_number = start_line_number;              // copy the start line number
  memory_address = start_memory_address;        // copy the start memory address

while (fgets(str, sizeof(str), read_fp) != NULL){

  instruction_byte_size = detect_levels(str);
  memory_address += instruction_byte_size;

  if((!instruction_size) || (error_flag)) goto end;   // if error occurs, then compilation terminated
  line_number++;
}
 fclose(read_fp);


////////* Second Scanning to detect the instructions and write its equivalent hex code into the test.o file *//////

  read_fp = fopen("test.asm", "r");
  write_fp = fopen("test.o", "w");

  line_number = start_line_number;            // copy the start line number
  memory_address = start_memory_address;      // copy the start memory address

while (fgets(str, sizeof(str), read_fp) != NULL){
  //fgets(str, 50, read_fp);  /*The above condition is sufficient to read the line*/
  // printf("%s", str);  /* For Printing the line */

  direction_to_detect_instructions(str);

  if((!instruction_size) || (error_flag)){ instruction_size = 1; goto end;}   // if error occurs, then compilation terminated
  if(comment_flag){ comment_flag = 0; goto skip;}                             // if comment is there, then skip the line

/* This is for only 1 byte instruction */
  else if(instruction_size == 1){
    fprintf(write_fp, "%X:   ", memory_address++);
    fprintf(write_fp, "%X\n", opcode);
  }

/* This is for 2 byte instruction */
  else if(instruction_size == 2){
    fprintf(write_fp, "%X:   ", memory_address++);
    fprintf(write_fp, "%X\n", opcode);

    fprintf(write_fp, "%X:   ", memory_address++);
    fputs(data, write_fp);
    instruction_size = 1;
  }

/* This is for 3 byte instruction */
  else if(instruction_size == 3){
    fprintf(write_fp, "%X:   ", memory_address++);
    fprintf(write_fp, "%X\n", opcode);

    fprintf(write_fp, "%X:   ", memory_address++);
    fputs(low_byte_address, write_fp);

    fprintf(write_fp, "%X:   ", memory_address++);
    fputs(high_byte_address, write_fp);

    instruction_size = 1;
  }
  skip:
  line_number++;
}

printf("\n\n\n--------------------------------------------\n");
printf("|  Machine Code Generation - [Successful]  |");
printf("\n--------------------------------------------\n\n\n");

end:  fclose(read_fp);
      fclose(write_fp);

  if(error_flag){
    write_fp = fopen("test.o", "w");  // for earasing the all the generated machine code when error occurs
    fclose(write_fp);
  }

  return 0;
}

