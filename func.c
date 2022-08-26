#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <unistd.h> 
#include <fnmatch.h>
#include <getopt.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>
#include "func.h"

//primary functions

char **parseUserInput(int argc, char **argv)
{

    struct option long_options[] =
    {
        {"file", required_argument, NULL, 'f'},
        {"name", required_argument, NULL, 'n'},
        {"user", required_argument, NULL, 'u'},
        {"type", required_argument, NULL, 't'},
        {"print", 0, NULL, 'p'},
        {"ls", optional_argument, NULL, 'l'},
        {NULL, 0, NULL, 0}
    };

    int opt;

    char **arg_arr = malloc(sizeof(char*)*12);
    for(int i=0; i<12; i++){
        arg_arr[i]=""; 

    }
     
    


    while((opt = getopt_long(argc, argv, "f:n:b:c:hl:", long_options, NULL)) != -1) 
    { 
        switch(opt) 
        { 
			case 'n':
				arg_arr[3] = "n";
				arg_arr[4] = optarg;
				break;
			case 't':
				arg_arr[5] = "t";
				arg_arr[6] = optarg;
				break;
			case 'p':
				arg_arr[7] = "p";
				arg_arr[8] = optarg;
				break;
			case 'l':
				arg_arr[9] = "l";
				arg_arr[10] = optarg;
				break;
			case 'u':
				arg_arr[11] = "u";
				arg_arr[12] = optarg;
				break;
			case '?':
				printf("\nType -h for help\n\n");
				exit(0);
			case 'h':
				printHelp();
				exit(0);
			default:   
                arg_arr[3] = "n";
		        arg_arr[4] = "*"; 
                break;
			case ':':
                switch (optopt)
                {
                case 'l':
                    //printf("option -%c with default argument value\n", optopt);
                    arg_arr[10]="*";
                    break;
                }
            break;
        } 
    }

    if (argc==2||argc==1){
     
	}
    for (int index = optind; index < argc; index++)
        arg_arr[0]=argv[index];
    if(!strcmp(arg_arr[0], "")||arg_arr[0]==NULL)  
        arg_arr[0] ="./";
    //return 0;
    if(arg_arr[9][0]=='l'){
        if(arg_arr[10]==NULL){
            arg_arr[10] = "noArg";
        }

    }
    
    return arg_arr;
}

void do_entry(char *entry_name, char **parms)
{
    
    struct stat filestat;
    stat(entry_name, &filestat);
    //printf("hi");
    int printflag = 1;
    if(entry_name == NULL){
        return;
    }

    if(parms[3][0]=='n')
    {
        if(fnmatch(parms[4], entry_name, 0))
            printflag = 0;
    }
    //findByUser
    if(parms[11][0]=='u'&&fnmatch(parms[12], getpwuid(filestat.st_uid)->pw_name, 0)){
        printflag = 0;
    }
    if(parms[5][0]=='t'){
        printflag = findByType(filestat.st_mode, parms[6], printflag);
    }
    if(parms[9][0]=='l'){
        if(strcmp(parms[10], "noArg")){
            if(fnmatch(parms[10], entry_name, 0))
            printflag = 0;
        }
    }

    if(printflag){

        printPath(entry_name);
        if(parms[9][0]=='l'){
            listEntryDetails(entry_name);
        }
    }

    return;
}

void do_dir(const char *dir_name, char ** parms)
{

    
    struct stat filestat;
    struct dirent *entry;
    int filesCounter = 0;
    
    DIR *folder = opendir(dir_name);

    
    chdir(dir_name);
    if(folder == NULL)
    {
        puts("Unable to read directory");
        return;
    }
     
    if(parms[7][0]=='p'){
        printAllDirEntries(dir_name);
        //return;
    }
    
    //printf("%4s: %s\n","Name",entry->d_name);
    //read all of the directory entries
    while( (entry=readdir(folder)) ){

        filesCounter++;

        stat(entry->d_name, &filestat); 
        
        
        //printf("%4s: %s\n","Name",entry->d_name);

        if( S_ISDIR(filestat.st_mode)){

            if((!strcmp(entry->d_name, ".."))||(!strcmp(entry->d_name, "."))){
                continue;
            }     
            //check if the name matches
            
            do_entry(entry->d_name, parms);
            //go through the directory
            do_dir(entry->d_name, parms);
        } 
        else{
            //printf("%4s: %s\n","File",entry->d_name);
            do_entry(entry->d_name, parms);
        }
        
    }
    closedir(folder);
    //if(strcmp(dir_name, "./")){
        chdir("..");
    
    return;

}

//helper functions

int findByType(mode_t fileMode, char *typeToPrint, int printFlag)
{


    if((!strcmp (typeToPrint, "f")))
    {
        if(!S_ISREG(fileMode)){
            printFlag = 0;

        }
    }
    else if((!strcmp (typeToPrint, "d")))
    {
        if(!S_ISDIR(fileMode)){
            printFlag = 0;
        }
    }
    else if( (!strcmp (typeToPrint, "c")))
    {
        if(!S_ISCHR(fileMode)){
            printFlag = 0;
        }
    }

    else if( (!strcmp (typeToPrint, "b")))
       {
        if(!S_ISBLK(fileMode)){
            printFlag = 0;
        }
    }

    else if( (!strcmp (typeToPrint, "p")))
        {
        if(!S_ISFIFO(fileMode)){
            printFlag = 0;
        }
    }

    else if( (!strcmp (typeToPrint, "l")))
    {
        if(!S_ISLNK(fileMode)){
            printFlag = 0;
        }
    }
    else if( (!strcmp (typeToPrint, "s")))
    {
        if(!S_ISSOCK(fileMode)){
            printFlag = 0;
        }
    }
    return printFlag;
}

void printPath(char *entryName)
{
    char src[255] = "";
    char relativePath[255]= "";
    strcpy(src,  entryName);
    strcpy(relativePath, "./");
    strcat(relativePath, src);
    char *path = realpath(relativePath, NULL);
    printf("%s\n", path);
    free(path);
}

void listEntryDetails(char *entryName)
{
    struct stat filestat;
    stat(entryName, &filestat);
    printf("%lu   ", filestat.st_ino);
    printf("%lu   ", filestat.st_blocks);
    printPermissions(entryName);
    printf("%lu   ", filestat.st_nlink);
    printf("%s   ", getpwuid(filestat.st_uid)->pw_name);
    printf("%s   ", getpwuid(filestat.st_gid)->pw_name);
    printDateAndTime(filestat.st_mtim.tv_sec);
    printf("%s", entryName);
    printf("\n");
}

void printAllDirEntries(const char *path)
{
    if(strcmp(path,".")||strcmp(path,"..")){
        return;
    }
    DIR *folder = opendir(path);
    struct dirent *entry;
    if(folder == NULL)
    {
        puts("Unable to read directory");
        return;
    }
    //read all of the directory entries
    while( (entry=readdir(folder)) )
    {
        printPath(entry->d_name);
    }
    closedir(folder);
    return;
}

void printPermissions(char *entryName)
{
    struct stat fileStat;
    stat(entryName, &fileStat);
    printf("   ");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("   ");

}

void printDateAndTime(unsigned int time)
{
    time_t t = time;
    struct tm *tmp = localtime(&t);
    char outstr[200];
    //style of the time
    char * timeStyle = "%a, %d %b %Y %T %z" ; 
    strftime(outstr, sizeof(outstr), timeStyle, tmp);
    printf("   %s   ", outstr);
}

void printHelp() 
{
    printf("\n");
    printf("  USAGE: ./<programm_name> --option argument(s) --option argument(s) --option argument(s) \n");
    printf("  Options:\n");
    printf("  --name <regex expression>   //prints the paths of all the matching directory entries\n");
    printf("  --print //prints all entries from a directory and its child directories\n");
    printf("  --ls==<regex expression for filtering files> (optional argument - used when not in combination wth other options)\n"
    "//prints the detailed information of all the matching directory entries;\n"
     "usually used in combination with other options, without arguments\n");
    printf("  --user <userName> \n //prints the paths of all the directory entries, that are owned by matching users \n");
    printf("  --type \"type argum" //prints the paths of all the directory entries, that are of a matching type\n"
    	   "        Possible types: \n"
            "regular file: 'r', directory: 'd', block: 'b', pipes: 'f', links: 'l', sockets: 's', \n");
   
    printf("  -h display this message (invalidates next options)\n");
    printf("  ./myFind ./ --name \"mock*\" --ls\n\n");
    exit(0);
}