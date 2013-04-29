#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#define eprintf(...) fprintf (stderr, __VA_ARGS__)

void * memcpy ( void * destination, const void * source, size_t num );
size_t strlen ( const char * str );
time_t time ( time_t * timer );

// from [0,rand)
int myrand(int max) {
  return rand() % max;
}

void rand_permute_map(void* map2, size_t size, int maplen) {
  char* map = (char*)map2;
  int total = myrand(100)+50; // do 50-149 random swaps of mappings
  int i, j, j2, t;
  for (i = 0; i < total; i++) {
    j = myrand(maplen);
    j2 = myrand(maplen);
    t = map[j*size];
    map[j*size] = map[j2*size];
    map[j2*size] = t;
  }
}

// randomly permute
void do_permute(char* str)
{
  int i, len = strlen(str);
  {  
  int map[len];
  char astrcpy[len];

  for (i = 0; i < len; i++) // init perm map
    map[i] = i;

  rand_permute_map(map, sizeof(int), len);
  strcpy(astrcpy, str);

  for (i = 0; i < len; i++)
    str[i] = astrcpy[map[i]];
  for (i = 0; i < len; i++)
    eprintf("%3d -> %3d: %c %c\n",i,map[i],str[i], astrcpy[i]);
  }
  //eprintf("LENGTH: %d\n",len);
}

int cccomp(const void* a, const void* b) { return *(char*)a - *(char*)b; }

// Note: upper and lower case are treated the same
//       No special treatment of spaces
void do_substitute(char* str) {
  int i, len = strlen(str);
  char mapto[26];

  for (i = 0; i < 26; i++)
    mapto[i] = 'a'+i;

  rand_permute_map(mapto, sizeof(char), 26);
  // print the mappings
  for (i = 0; i < 26; i++)
    eprintf("%3d: %c -> %c\n",i,'a'+i,mapto[i]);
  
  // now do the substitution
  for (i = 0; i < len; i++) {
    if (!isalpha(str[i]))
      continue;
    char c = tolower(str[i]);
    str[i] = mapto[c-'a'];
  }

}

// special case: do a Caesar shift permutation
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
    else if (!isspace(str[i]))
      eprintf("%c occurred in do_shift and is not a letter or space.  Not shifting.\n",str[i]);
  }
}

// free this
// result is '\0'-terminated
char* make_big_string(char** strarr, unsigned numstr) {
  unsigned i, j, outpos;
  unsigned len = 0;
  char* strout;
  for (i = 0; i < numstr; i++)
    len += strlen(strarr[i])+(i > 0 ? 1 : 0);
  len++; // for '\0'
  strout = (char*)malloc(len*sizeof(char));
  for (i = j = outpos = 0; i < numstr; i++) {
    for (j = 0; j < strlen(strarr[i]); j++)
      strout[outpos++] = strarr[i][j];
    if (i != numstr-1)
      strout[outpos++] = ' ';
  }
  strout[outpos++] = '\0';
  assert(outpos == len);
  return strout;
}

void print_usage_exit(const char* progname) {
  printf("Usage: %s [-seed rand_seed] [-permute | -substitute | -shift] strings...\n"
	 "\tPermute changes character positions (diffusion)\n"
	 "\tSubstitute changes LETTERS' identities (confusion)\n"
	 "\tShift is a special case of substitute (Caesar cipher). Shifts LETTERS *seed* places.\n", progname);
  exit(0);
}

int main(int argc, char** argv)
{
  int seed = 0;
  char *big, *progname = argv[0];
  void (*op)(char*); op = 0;

  argc--;
  argv++;
  while (argc > 0 && argv[0][0] == '-') {
    if (strcmp(argv[0], "-seed") == 0) {
      seed = atoi(argv[1]);
      argc--; // 1 more for the argument
      argv++;
    }
    else if (strcmp(argv[0], "-permute") == 0)
      op = &do_permute;
    else if (strcmp(argv[0], "-substitute") == 0)
      op = &do_substitute;
    else if (strcmp(argv[0], "-shift") == 0)
      op = (void (*)(char*))1;	/* special marker for do_shift (different signature than op) */
    else
      print_usage_exit(progname);
    argc--;
    argv++;
  }
  if (!op)
    print_usage_exit(progname);
  if (argc < 1)
    print_usage_exit(progname);

  big = make_big_string(argv, argc);
  if (op == (void (*)(char*))1)
    do_shift(big, seed);    	/* seed is the number to shift, not used to seed the random number gen */
  else {
    if (!seed)
      seed = time(0);
    srand(seed);
    (*op)(big);
  }

  printf("%s",big);
  printf("\n");
  free(big);
  return 0;
}
