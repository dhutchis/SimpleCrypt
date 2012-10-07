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
  {  int map[len];
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
  eprintf("LEN:%d\n",len);
}

int cccomp(const void* a, const void* b) { return *(char*)a - *(char*)b; }

// Note: upper and lower case are treated the same
//       No special treatment of spaces
void do_substitute(char* str) {
  int i, len = strlen(str);
  //char uniqchars[len]; // contains the unique characters of str
  /*
  // first, copy str to uniquechars, sort uniqchars, and eliminate repeat chars, keep track of total length
  (void)memcpy(uniqchars, str, len);
  qsort(uniqchars, len, sizeof(char), &cccomp);
  int num_uniq = len;
  for (i = 0, j = 1; j < len; ) {
    while (uniqchars[i] == uniqchars[j]) {
      j++;
      num_uniq--;
    }
    uniqchars[++i] = uniqchars[j];
    j++;
  }*/

  // map from uniqchars to mapto; start with trivial map same->same
  //  char mapto[num_uniq];
  char mapto[26];
  //  (void)memcpy(mapto,uniqchars,num_uniq);
  for (i = 0; i < 26; i++)
    mapto[i] = 'a'+i;
  //for (i = 0; i < num_uniq; i++)
    //    mapto[i] = uniqchars[i];
  // permute the mappings
  //  rand_permute_map(mapto, sizeof(char), num_uniq);
  rand_permute_map(mapto, sizeof(char), 26);
  // print the mappings
  //  for (i = 0; i < num_uniq; i++)
  //    eprintf("%3d: %c -> %c\n",i,uniqchars[i],mapto[i]);
  for (i = 0; i < 26; i++)
    eprintf("%3d: %c -> %c\n",i,'a'+i,mapto[i]);
  // now do the substitution
  for (i = 0; i < len; i++) {
    if (!isalpha(str[i]))
      continue;
    char c = tolower(str[i]);
    //    eprintf("search for %c\n",c);
    //char* cp = bsearch(&str[i], uniqchars, num_uniq, sizeof(char), &cccomp);
    //char* cp = bsearch(&c, mapto, 26, sizeof(char), &cccomp);
    //eprintf("%d: str[i]:%c cp:%x cp-uniqchars:%x\n",i,str[i],cp,cp-mapto);
    
    //str[i] = mapto[ cp - mapto ];
    str[i] = mapto[c-'a'];
  }

}

/*void do_permute_multi(char** strarr, int numstr)
{
  int i;
  for (i = 0; i < numstr; i++) {
    //    printf("%s ",strarr[i]);
    do_permute(strarr[i]);
  }
  //printf("\n");
  }*/

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

void print_usage_exit() {
  printf("Usage: do_permute [-seed rand_seed] [-permute | -substitute] strings...\n");
  exit(0);
}

int main(int argc, char** argv)
{
  int seed = 0;
  char* big;
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
    else
      print_usage_exit();
    argc--;
    argv++;
  }
  if (!op)
    print_usage_exit();
  if (!seed)
    seed = time(0);
  srand(seed);
  if (argc < 1)
    print_usage_exit();

  big = make_big_string(argv, argc);
  (*op)(big);

  printf("%s",big);
  printf("\n");
  free(big);
  return 0;
}
