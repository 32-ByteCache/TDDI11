#include <libepc.h>
#include <stdio.h>

void llmultiply(unsigned long long int l1,
                unsigned long long int l2,
                unsigned char *result);




struct test_case {
  unsigned long long int a;
  unsigned long long int b;
  unsigned long long int rh;
  unsigned long long int rl;
};

struct test_case cases[6] = {

  { 0x0000111122223333ULL, 0x0000555566667777ULL,
    0x0000000005B061D9ULL, 0x58BF0ECA7C0481B5ULL },

  { 0x3456FEDCAAAA1000ULL, 0xEDBA00112233FF01ULL,
    0x309A912AF7188C57ULL, 0xE62072DD409A1000ULL },

  { 0xFFFFEEEEDDDDCCCCULL, 0xBBBBAAAA99998888ULL,
    0xBBBB9E2692C5DDDCULL, 0xC28F7531048D2C60ULL },

  { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL,
    0xFFFFFFFFFFFFFFFEULL, 0x0000000000000001ULL },
  
  { 0x00000001FFFFFFFFULL, 0x00000001FFFFFFFFULL,
    0x0000000000000003ULL, 0xFFFFFFFC00000001ULL },

  { 0xFFFEFFFFFFFFFFFFULL, 0xFFFF0001FFFFFFFFULL,
    0xFFFE0002FFFDFFFEULL, 0x0001FFFE00000001ULL }
};

void PutUnsignedLongLong(unsigned long long int* ulli)
{
  unsigned long int* uli = (unsigned long int*)ulli; 
  PutUnsigned(uli[1], 16, 8);
  PutUnsigned(uli[0], 16, 8); 
}

void checkCarry(unsigned long int* ptr, int pos, unsigned long long b) {
  if ((ptr[pos] + b) < ptr[pos]) {
    ptr[pos+1] += 1;
    PutString("carry!");
  }
}



void cllmultiply(unsigned long long int l1, unsigned long long int l2, unsigned char *result) {
  unsigned long long int mult_total;
  unsigned long int mult_h;
  unsigned long int mult_l;

  unsigned long long int a_low = (unsigned long int)l1;
  unsigned long long int a_high = (l1 >> 32);

  unsigned long long int b_low = (unsigned long int)l2; 
  unsigned long long int b_high = (l2 >> 32);

  unsigned long int *res_ptr = result;

  res_ptr[0] = 0;
  res_ptr[1] = 0;
  res_ptr[2] = 0;
  res_ptr[3] = 0;




  mult_total = a_low * b_low;
  mult_l = (unsigned long int)mult_total; 
  mult_h = (mult_total >> 32);


  

  res_ptr[0] = mult_l;
  res_ptr[1] = mult_h;

  mult_total = a_low * b_high;
  mult_l = (unsigned long int)mult_total; 
  mult_h = (mult_total >> 32);

  
  if ((res_ptr[1] + mult_l) < res_ptr[1]) {
    res_ptr[2] += 1;
  }
   


  res_ptr[1] += mult_l;
  res_ptr[2] += mult_h;

  mult_total = a_high * b_low;
  mult_l = (unsigned long int)mult_total; 
  mult_h = (mult_total >> 32);

  if ((res_ptr[1] + mult_l) < res_ptr[1]) {
    res_ptr[2] += 1;
  }

  if ((res_ptr[2] + mult_h) < res_ptr[2]) {
    res_ptr[3] += 1;
  }

  res_ptr[1] += mult_l;
  res_ptr[2] += mult_h;

  mult_total = a_high * b_high;
  mult_l = (unsigned long int)mult_total; 
  mult_h = (mult_total >> 32);

  if ((res_ptr[2] + mult_l) < res_ptr[2]) {
      res_ptr[3] += 1;
  }
    
  res_ptr[2] += mult_l;
  res_ptr[3] += mult_h;


  
};

int main(int argc, char *argv[])
{
  unsigned char result[16];
  int i;
  
  ClearScreen(0x07);
  SetCursorPosition(0, 0);

  Milliseconds();

  for (i = 0; i < 6; ++i)
  {
    PutString("Test : ");
    PutUnsignedLongLong(&cases[i].a);
    PutString(" * ");
    PutUnsignedLongLong(&cases[i].b); 
    PutString("\r\n");
   
    PutString("    == ");
    PutUnsignedLongLong(&cases[i].rh);
    PutUnsignedLongLong(&cases[i].rl);
    PutString("\r\n");
    
    cllmultiply(cases[i].a, cases[i].b, result);
    
    PutString("Result ");
    PutUnsignedLongLong((unsigned long long int*)&result[8]);
    PutUnsignedLongLong((unsigned long long int*)&result[0]); 
    PutString("\r\n");
    
    PutString("\r\n");
  }
  PutString("Time: ");
  PutUnsigned(Milliseconds(), 10, 10);

  return 0;
}
