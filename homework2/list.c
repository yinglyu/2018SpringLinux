#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h> 
#include <time.h>
#define DAY_SEC 86400
#define BUFLEN 255 
time_t now;
long int time_limit;
int errflg = 0, helpflg = 0, lflg = 0, hflg = 0, mflg = 0, rflg = 0, aflg = 0;
int low = 0, high = 1000000, modify = 100;

int recursion_dir(char * path){
	struct dirent *entry;
	struct stat st;
	DIR *dir;
	int ret = 0;
	char  new_path[BUFLEN],format_time[BUFLEN];
	dir = opendir(path);
	if (dir == NULL) {
            printf("Open directory %s: %s (ERROR %d)\n",path, strerror(errno), errno);
        return 1;//errno 
        }
    while ((entry = readdir(dir)) != NULL){
 		sprintf(new_path,"%s/%s",path,entry->d_name);
        ret = stat(new_path,&st);
		/*printf("%s\n",new_path);test*/
        if (ret == -1){
			printf("error: %m\n");
		}
		if(!S_ISDIR(st.st_mode)&
				((mflg&(st.st_mtime>time_limit))|(!mflg))&
                (lflg&(st.st_size>low)|(!lflg))&
                (hflg&(st.st_size<high)|(!hflg))
                 )
        {
			strftime(format_time, BUFLEN, "%F", localtime(&(st.st_mtime)));
			printf("%s\t\t",format_time);
			printf("%ld\t\t",st.st_size);
            printf("%s\n",  entry->d_name);
			//printf("%ld\t\t",st.st_mtime);

        }
	//	printf("d_name%s\n",entry->d_name);
        if(S_ISDIR(st.st_mode)&(entry->d_name[0]!= '.')){
        	
			printf("%s:\n",new_path);
             recursion_dir(new_path);
			printf("%s end\n",new_path);          
        }
    }
	closedir(dir);

}

int main(int argc, char **argv)
{
	char *path,format_time[BUFLEN],new_path[BUFLEN];//单纯的 节点无法访问../目录
	DIR *dir;
	struct dirent *entry; //dirent.h
 	struct stat st;//<sys/stat.h>
	int ret;
	int c;
	
 	
  struct option longopts[] =//getopt.h
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
        	printf("low = %d B\t",low);
        	lflg = 1;
        break;
      	case 'h':
    		high = atoi(optarg);
        	printf("high = %d B\t",high);
        	hflg = 1;
        break;
      	case 'm':
    		modify = atoi(optarg);
        	printf("modify within %d day(s)\t",modify);
        	mflg = 1;
        break;
      	case 'r':
			printf("recursion\t");
			rflg = 1;
        break;
      	case 'a':
        	printf("all\t");
        	aflg = 1;
    	break;

      	case '?':
        	errflg++;
        break;
      	default:
        	
        break;
    }
  }
    if (mflg){
       time(&now);
       strftime(format_time, BUFLEN, "%F", localtime(&now));
       printf("today date : %s\t\t",format_time);
       time_limit = now - modify*DAY_SEC;
    }
	printf("optind:%d\t",optind);/*第一个非选项参数出现位置*/
	printf("argc:%d\n",argc);/*包括选项的参数字段总个数+1*/
	if(optind == argc){
		//printf("0 arg\n");
		argv[optind] = ".";
		argc ++;
	}

		while (optind < argc){
			
	
			path = argv[optind];
		//	printf("%s:\n",path);
			ret = stat(path,&st);
			if (ret == -1){
              printf("error: %m\n");
      		}
			if(S_ISREG(st.st_mode)){
				//printf("is file\n");
		
        		if(((mflg&(st.st_mtime>time_limit))|(!mflg))&
                (lflg&(st.st_size>low)|(!lflg))&
                (hflg&(st.st_size<high)|(!hflg))
                 ){
					strftime(format_time, BUFLEN, "%F", localtime(&(st.st_mtime)));
					printf("%s\t\t",format_time);
					printf("%ld\t\t",st.st_size);
            		printf("%s\n",  path);
					//printf("%ld\t\t",st.st_mtime);
        		}	
			}
			else if(S_ISDIR(st.st_mode)){
				//printf("is dir\n");
				dir = opendir(path);
				if (dir == NULL) {
					printf("Open directory%s: %s (ERROR %s)\n", 
						argv[optind], strerror(errno), errno);
				return 1;//errno 
			}//dirent
			if (rflg){
				printf("start recursion\n");
				recursion_dir(path);
			}
			else
				while ((entry = readdir(dir)) != NULL){
					/*printf("158\n");test*/
 					sprintf(new_path,"%s/%s",path,entry->d_name);
        			ret = stat(new_path,&st);//lstat stat
			
					if (ret == -1){
               			printf("error: %m\n");
					}
					else if(!S_ISDIR(st.st_mode)&((mflg&(st.st_mtime>time_limit))|(!mflg))&
						(lflg&(st.st_size>low)|(!lflg))&
						(hflg&(st.st_size<high)|(!hflg))&
						(!aflg&(entry->d_name[0]!='.')|(aflg))
						){    		
						strftime(format_time, BUFLEN, "%F", localtime(&(st.st_mtime)));
						printf("%s\t\t",format_time);
						printf("%ld\t\t",st.st_size);
            			printf("%s\n",  entry->d_name);
						//printf("%ld\t\t",st.st_mtime);
					
					}
				}
		
			closedir(dir);
	
			}
			optind++;
		}
	
  if(errflg) 
  {//stdio.h
     fprintf(stderr,"Usage:    ./list_v6 [OPTION]\n\n",
    "Mandatory arguments to long options are mandatory for short options too.\n",
    "\t-r, --recursion   show the subfile\n ",
    "\t-a, --all         show the file start with dot\n",
    "\t-l, --low        minimum of size, default is 0\n",
    "\t-h, --high        maximum of size, default is\n",
    "\t-m, --modify     modify date\n",
    "\t    --help        print this help\n\n",

    "Report bugs to <lyuying@bupt.edu.cn>.\n"
     );
    
  }


  return 0;
} 

