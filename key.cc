// key generator and examiner
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>

#define KEY_SIZE 59
#define MAXKEYS 55
#define SIGNATURE_SIZE 9
#define LOW_GOAL 1755
#define HIGH_GOAL 2001

using namespace std;

// variables
int repetitions, generations;
char signature[SIGNATURE_SIZE];
char first_codes[SIGNATURE_SIZE+2];
int signature_value;

// routines
int examine_key();
int generate_key();
void arrange_value_of_specific_array(int size, int goal, char codes[KEY_SIZE]);
int check_signature();
int dismissnondouble(double param);

// main routine
int main(int argc, char *argv[])
{
  int i;
  srand(time(NULL));
  
  // if no flag is declared, exit
  if (argc<3 || argv[1][0]!='-') {
   printf("usage: key -g(enerate) <repetitions> or\n");
   printf("           -e(xamine) <repetitions>\n");
  exit (-1); }
  // determine repetitions
  if (!atoi(argv[2]))
    repetitions=1;
  else 
   repetitions=atoi(argv[2]); 
   
   // examine key routines
   if (argv[1][1]=='e') {
    printf("signature (8 chars):");
    cin >> signature;
    if (!check_signature()) {
     printf("invalid signature\n");
    exit (-1); }
    for (generations=0;generations<repetitions;generations++) { 
     if (examine_key())
      printf("\nkey is correct!"); 
     else 
      printf("\nkey is wrong."); }
    printf("\n");
   exit (1); }
   
  // generate key routines  
  if (argv[1][1]=='g') { 
   printf("signature (8 chars):");
   cin >> signature;
   if (!check_signature()) {
    printf("invalid signature\n");
   exit (-1); }
   signature_value=0;
   for (i=0;i<SIGNATURE_SIZE;i++) 
    signature_value+=(int) signature[i];
   for (generations=0;generations<repetitions;generations++) {
    arrange_value_of_specific_array(11, signature_value, first_codes);
   generate_key(); }
   printf("\n");
  exit (1); }
  
  // other possibilities from command line
  printf("invalid flag.\n");

  return 0;
}

// analyze key
int examine_key()
{
  int i,n;
  int pos[MAXKEYS];
  int total_value; 
  char key[KEY_SIZE+1];
  int goal_value;
  
   printf("\nkey:");
   cin >> key;
   
   // read signature
   signature_value=0;
   for (i=0;i<SIGNATURE_SIZE;i++) 
    signature_value+=(int) signature[i];
   n=0;
   for (i=2;i<SIGNATURE_SIZE+4;i++) 
    n+=(int) key[i];
   if (n!=signature_value)
    return 0;
  
   // read last 15 chars that will determine total value goal
   goal_value=0;
   for (i=KEY_SIZE-15;i<KEY_SIZE;i++) 
     goal_value+=(int) key[i];
   goal_value*=2;
  
  i=1;
  n=0;
  total_value=0;
  total_value+=(int) key[0];
  pos[n]=(int) key[0]/3;
   while (i<KEY_SIZE) {
    if (i==pos[n]) {
     total_value+=(int) key[pos[n]]; 
     pos[n+1]=(int) key[pos[n]]/3; 
     if (total_value==goal_value)
      break;
    ++n; }
   ++i; }

   // exception for when the end flag char is lost because total value chars start
   if (i>KEY_SIZE-22)
    i=0;
   // return true if everything is alright
   if (total_value==goal_value)
    return 1; 
   
 return 0;
}

// key generator
int generate_key()
{
  int i,n,l,k;
  char key[KEY_SIZE+1];
  char keys[MAXKEYS];
  int key_positions[MAXKEYS];
  int key_value;
  char last_codes[KEY_SIZE];
  int goal_value;

   goal_value=0;
    while (goal_value<LOW_GOAL || goal_value>HIGH_GOAL)
     goal_value=rand() % HIGH_GOAL; 
    if (!dismissnondouble((double) goal_value/2))
     --goal_value;
  
    key_value=0;
    while(key_value!=goal_value) {
   
    // generate keys 
    for (i=0;i<MAXKEYS;i++) { 
     while (n<48 || n>126) {
      n=rand() % 126;
      for (l=0;l<i;l++)
       if ((int) keys[l]==n)
      n=0; }
     keys[i]=(char) n;
    n=0; }
   
    // sort keys array
    for (i=0;i<MAXKEYS;i++) 
     for (n=MAXKEYS-1;n>-1;n--)  
      if ((int)keys[n-1]>(int)keys[n]) {
       l=keys[n];
       keys[n]=keys[n-1];
       keys[n-1]=l; } 
     for (i=0;i<MAXKEYS;i++) 
      key_positions[i]=(int) keys[i]/3; 
    
     key_value=0;
    // generate key 
    i=0;
    l=0;
    k=0;
    key[i]=keys[l];
    key_value+=(int)toupper(key[i]);
    ++i;
    ++l;
    while (i<KEY_SIZE) {
     while (key_positions[l]==key_positions[l-1])
      ++l;
     if (i==key_positions[l-1] && key_value<goal_value && l<MAXKEYS) {
      key[i]=keys[l];
      key_value+=(int)(key[i]);
     ++l; }
     else {
      n=0;
      while (n<48 || n>126) 
       n=rand() % 126;
     key[i]=(char) n; }
    ++i; }  }

    // place signature in key from [2] to [13]
    n=0;
    for (i=2;i<SIGNATURE_SIZE+4;i++) {
     key[i]=first_codes[n];
     ++n; }
   // arrange last 15 chars with value of GOAL/2
    arrange_value_of_specific_array(15, goal_value/2, last_codes);
    n=0;
    for (i=KEY_SIZE-15;i<KEY_SIZE;i++) {
     key[i]=last_codes[n];
    ++n; }
    key[++i]='\0';
    printf("\n%s", key);
 
 return 1;
}

// arrange array of chars with total requested value  
void arrange_value_of_specific_array(int size, int goal, char codes[KEY_SIZE])
{
   int i,n;
   int total_value;
  
   while (total_value!=goal) {
    total_value=0;
    for (i=0;i<size;i++) {
    while (n<48 || n>126) 
     n=rand() % 126;
     codes[i]=(char) n;
     total_value+=n;
    n=0; } }
}

// check structure of signature string
int check_signature()
{
  int i;
   
   for (i=0;i<SIGNATURE_SIZE;i++)
    if (!isalpha(signature[i]) && !isdigit(signature[i]))
     return 0;

 return 1;
}

// dismiss value with decimal part
int dismissnondouble(double param)
{
  double fractpart, intpart;
  
   fractpart=modf(param, &intpart);
   if (!(fractpart*10)) 
    return 1; 

 return 0;
}
