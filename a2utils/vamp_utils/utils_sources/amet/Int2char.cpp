char* int2char(unsigned int, char*);

void main(void)

{
   unsigned int a;
   char String[4]; // String for conversion

   cout << "\nInput value: "; cin >> a;
   cout << "Your value is " << int2char(a, String);
   cout << "\n";
}


char* int2char(unsigned int a, char* String)

{
  for (char i = 0; i < 4; i++) String[i] = '\0';

  if (a > 999) return String;

  if (a >= 100 && a <= 999)
  {
     String[0] = a / 100 + '0';
     String[1] = a / 10 - a / 100 * 10 + '0';
     String[2] = a - a / 10 * 10 + '0';
     return String;
  }

  if (a >= 10 && a <= 99)
  {
     String[0] = a / 10 + '0';
     String[1] = a - a / 10 * 10 + '0';
     return String;
  }

  if (a <= 9) String[0] = a + '0';
  return String;
}