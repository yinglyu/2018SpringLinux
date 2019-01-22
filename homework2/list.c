#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

/* list main function */
int main(int argc, char **argv)
{
	DIR *dir;
	struct dirent *entry; 
	int c;
	int low = 0, high = 1000000, modify = 100;
	int errflg = 0, helpflg = 0, lflg = 0, hflg = 0, mflg = 0, rflg = 0, aflg = 0;
    	
  struct option longopts[] =
  {
    {"low", 1, 0, 'l'},
    {"high", 1, 0, 'h'},
    {"modify", 1, 0, 'm'},
    {"recursion", 0, 0, 'r'},
    {"all", 0, 0, 'a'},
    {"help", 0, &helpflg, 1},
    {0, 0, 0, 0}
  };

  while ((c = getopt_long (argc, argv, "l:h:m:ral", longopts, NULL)) != EOF)
  {
    switch(c)
    {
    	case 'l':
    		low = atoi(optarg);
        	printf("low = %d B\n",low);
        	lflg = 1;
        break;
      	case 'h':
    		high = atoi(optarg);
        	printf("high = %d B\n",high);
        	hflg = 1;
        break;
      	case 'm':
    		modify = atoi(optarg);
        	printf("modify within %d day(s)\n",modify);
        	mflg = 1;
        break;
      	case 'r':
			printf("recursion");
			rflg = 1;
        break;
      	case 'a':
        	printf("all");
        	aflg = 1;
    	break;

      	case '?':
        	errflg++;
        break;
      	default:
        	
        break;
    }
  }
	dir = opendir(argv[optind]); 
	if (dir == NULL) {
		printf("Open directory \"%s\": %s (ERROR %d)\n", 
			argv[optind], strerror(errno), errno);
	return 1; 
	}
	while ((entry = readdir(dir)) != NULL)
		printf("%4d %s \n", entry->d_ino, entry->d_name);
	closedir(dir);
  if(errflg) 
  {
     fprintf(stderr,"Usage:    wsiod [OPTION]\n\n",
         "WSIO server based on web service.\n",
    "Mandatory arguments to long options are mandatory for short options too.\n",
    "\t-r, --recursion   show the subfile\n ",
    "\t-a, --all         show the file start with dot\n",
    "\t-l, --low        minimum of size, default is 0\n",
    "\t-h, --high        maximum of size, default is\n",
    "\t-m, --modify     modify date\n",
    "\t    --help        print this help\n\n",

    "Report bugs to <lyuying@bupt.edu.cn>.\n"
     );
    exit(0);
  }

  /* 省略部分 */

  return 0;
} 