#include <stdlib.h>
#include <stdio.h>

// shift num positions right
// letters only
void do_shift(char* str, int num)
{
  int i;

  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {

      if (str[i] > 'Z' - num)
	str[i] -= 'Z' - 'A' + 1;
      else if (str[i] < 'A' - num)
	str[i] += 'Z' - 'A' + 1;
      str[i] += num;
    }
    else if (str[i] >= 'a' && str[i] <= 'z') {

      if (str[i] > 'z' - num)
	str[i] -= 'z' - 'a' + 1;
      else if (str[i] < 'a' - num)
	str[i] += 'z' - 'a' + 1;
      str[i] += num;
    }
    else printf("%c occurred in do_shift and is not a letter.  Not shifting.\n",str[i]);
  }

}

void do_shift_arr(char** strarr, int numstr, int numshift)
{
  int i;


  for (i = 0; i < numstr; i++) {
    //    printf("%s ",strarr[i]);
    do_shift(strarr[i], numshift);
  }
  //printf("\n");
}

int main(int argc, char** argv)
{
  int i, n;
  char* big;

  argc--;
  argv++;
  if (argc < 2) {
    printf("Usage: do_shift num_to_shift strings...\n");
    exit(0);
  }
  /*
  for (i = 0; i < argc - 1; i++)
    printf("%s ",*(argv+1+i));
  printf("\n");
  */
  do_shift_arr(argv + 1, argc - 1, atoi(argv[0]));

  for (i = 0; i < argc - 1; i++)
    printf("%s ",*(argv+1+i));
  printf("\n");
}
