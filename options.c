#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "getopt.h"
#ifdef LALIB
#include "la_flag.h"
#else
#include "flag.h"
#endif

void usage(const char* progname)
{
  fprintf(stderr, "%s -o output_file -f -n array_size\n"
	  "  -f tells it to print compiler options\n"
	  "  -o is optional\n"
	  "  -n array_size required for computation.\n", progname);
  exit(0);
}


void options(int argc, char** argv,size_t* n, char** outfile)
{
  int opt_char = 0;
  int output_file_len = 0;
  char* output_file = 0;
  char* endptr = 0;
  size_t array_size = 0;
  
  *outfile = 0;

  if (argc<2) usage(argv[0]);

  while (1) {
    opt_char = getopt(argc,argv,"o:fn:h");
    if (-1==opt_char) break;

    switch (opt_char) {
    case 'h':
      usage(argv[0]);
      break;
    case 'o':
      output_file_len = strlen(optarg)+1;
      output_file = (char*) malloc(output_file_len);
      strncpy(output_file,optarg,output_file_len);
      *outfile = output_file;
      break;
    case 'f':
      printf("%s %s\n",argv[0], g_flag);
      break;
    case 'n':
      errno  = 0;
      array_size = strtoul(optarg, &endptr, 10);
      if (0!=errno || endptr == optarg) {
	fprintf(stderr, "The array size should be a positive integer.");
      } else {
	*n = array_size;
      }
      break;
    default:
      printf("unknown option\n");
      break;
    }
  }

  if (0==array_size) {
    exit(0);
  }
}

