/*
 * myfind.c
 *
 *    Programmname: myfind
 *    Aufgabenstellung: Funktionsprogrammierung des "find" Befehl
 *  Created on: Feb 26, 2020
 *      Author: Mahir Tulic   | 1910258009(ic19b009)
 *              David Rüb     | 1910258081(ic19b081)
 *              Manuel Houzar | 1910258089(ic19b089)
 *              Ibrahim Alnaif| 1910258033(ic19b033)
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <error.h>
#include <sys/stat.h>
#include <fnmatch.h>

#define MAXLEN 256
#define NULLCHAR 1


static void no_argv(int argc, char ** parms);
//do_entry(const char * entry_name, const char * const * parms);

static void do_entry(const char * entry_name, char ** parms);
static void do_dir(const char * dir_name, char ** parms);
static void do_nogroup(char *parms);
static void do_group(char *parms);
static void do_nouser(char *parms);
static void do_user(char *parms);
static void do_name(const char *parms);
static int do_type(const char *parms, const struct stat *entry_data);
static void do_path(char *parms);
static void do_print(char *parms);
static void do_ls(char *parms);

int main (int argc, char* argv[])
{
    //char* dirname
    no_argv(argc,argv);
    int argvlength = 0;
    char* dirname = NULL;
    dirname = argv[1];
    //dirname = ".";
    if (argv[1] != NULL) argvlength=strlen(argv[1]);
    //fprintf(stderr, "Test Länge: %d\n%c\n",argvlength,argv[1]);
    do_entry(argv[1],argv);
    //do_dir(dirname,argv);

    return 0;
}

static void no_argv(int argc, char ** parms){
	if (argc == 1) {
		 printf ("default directory ");}

	else if(argc >= 2 ){
		if (*parms[1] == '-'){
				 printf ("default directory ... %s sollte gegeben sein", parms[1]);
			 }}

}
void do_dir(const char * dir_name, char ** parms) {

    struct stat st;
    char wholepath[MAXLEN];
    errno=0;
    DIR *dirp;
    dirp = opendir(dir_name);
    //int is_dir_flag=0;
    char temp_dir[MAXLEN];
    if (errno != 0){
    //if ((dirp = opendir(dir_name)) == NULL) {
        
        error(1,errno, "Fehler!: %s\n",dir_name);
        return;
        //EXIT_FAILURE;
        
    }
    else {
        printf("wir haben %s gefunden \n",dir_name);
        const struct dirent *dirent;
        errno = 0;
        while ((dirent = readdir(dirp)) != NULL) {
            
            //printf("%d\n", testnr);
            if (strcmp(dirent->d_name, ".") != 0 && (strcmp(dirent->d_name, "..") != 0)) {
            printf("%s/%s\n", dir_name, dirent->d_name);
            snprintf(wholepath, (sizeof(dir_name)+sizeof(dirent->d_name) +NULLCHAR), "%s/%s", dir_name, dirent->d_name);
                if (lstat(wholepath, &st) == -1) {
                    error(0,errno, "stat - no such file or directory");
                    exit(1);
                }

            }
            if (S_ISDIR(st.st_mode)) {
                do_dir(wholepath,parms);
            }
    errno=0;
    }
        if (errno!=0){
            error(0,errno, "Fault while readdir");
        }
        if (closedir(dirp) == -1) {
            error(0,errno, "closedir");
            exit(1);
        }
    }
}


void do_entry(const char * entry_name, char ** parms)                                                                                                             
{                                                                                                                                                                 
    struct stat entry_data;
    /* get information about the file and/or directory
    if (lstat(entry_name, &entry_data) == -1)
    {
         error(0,errno,"lstat failed");
        return;
    }*/
    errno=0;                                                                                                                                                      
    int i=0;
    int do_dir_flag=1;                                                                                                                                                      
    char buffer[MAXLEN];                                                                                                                                          
    const char possible_entry[10][MAXLEN] = {"-nogroup","-group", "-nouser", "-user", "-name", "-type", "-path", "-print", "-ls"};                                
                                                                                                                                                                  
    while (parms[++i] != NULL){                                                                                                                                   
        if (*parms[i] == '-'){ 
            strcpy(buffer, parms[i]);                                                                                                                                   
            for (int j = 0; j < 10; j++) {                                                                                                                        
                                                                                                                                        
                if ((strcmp(possible_entry[j], buffer)) == 0) {                                                                                                   
                    printf("Gefunden:%s!!\n", possible_entry[j]); //                                                                                              
                                                                                                                                                               
                if (j == 0) {                                                                                                                                      
                                                                                                                                                                  
                   do_nogroup(parms[j + 1]);                                                                                                                      
                                                                                                                                                                  
                } else if (j == 1) {                                                                                                                               
                                                                                                                                                                  
                    do_group(parms[i + 1]);                                                                                                                       
                                                                                                                                                                  
                } else if (j == 2) {                                                                                                                               
                                                                                                                                                                  
                    do_nouser(parms[i + 1]);                                                                                                                      
                                                                                                                                                                  
                } else if (j == 3) {                                                                                                                               
                                                                                                                                                                  
                    do_user(parms[i + 1]);                                                                                                                        
                                                                                                                                                                  
                } else if (j == 4) {                                                                                                                               
                    do_dir_flag=0;                                                                                            
                    do_name(parms[i + 1]);
		} else if (j == 5) {
                	do_type(parms[i+1],&entry_data);          
                }
                                                                                                                                                                                                                                                                                                                  
                } else if (j == 6) {                                                                                                                               
                                                                                                                                                                  
                    do_print(parms[i]);                                                                                                                       
                                                                                                                                                                  
                } else if (j == 7) {                                                                                                                               
                                                                                                                                                                  
                    do_print(parms[i - 1]);                                                                                                                       
                                                                                                                                                                  
                } else if (j == 8) {                                                                                                                               
                                                                                                                                                                  
                    do_ls(parms[i - 1]);                                                                                                                          
                                                                                                                                                                  
                }else{
                   //hier noch error und vielleicht auch noch sowas wie help verwenden
                    exit(1);
                }   
                }                                                                                                                                                  
            }                                                                                                                                                     
        }                                                                                                                                                         
        else{     
            if (do_dir_flag==1){
            printf("\n nr:%i parameter:%s\n",i,parms[i]);                                                                                                                                                    
            do_dir(parms[i],parms);
            }                                                                                                                               
        }                                                                                                                                                         
    }                                                                                                                                                             
}
    static void do_group(char *parms){
        printf("Group: Gesucht wird nach: %s", parms);
    }
    static void do_nogroup(char *parms) {
        printf("NoGroup:Gesucht wird nach: %s", parms);
    }
    static void do_nouser(char *parms) {
        printf("do_nouser Gesucht wird nach: %s", parms);
    }
    static void do_user(char *parms) {
        printf(" do_user Gesucht wird nach: %s", parms);
    }
    static void do_name(const char *parms) {
        printf("do_name Gesucht wird nach: %s", parms);
    }
    
    static int do_type(const char *parms, const struct stat *entry_data) {
    	 char type_scan = parms[0];

    	    switch (type_scan) {
    	        case 'd':
    	            if ((entry_data->st_mode & S_IFMT) == S_IFDIR) {
    	                return 1;
    	            }
    	            break;
    	        case 'c':
    	            if ((entry_data->st_mode & S_IFMT) == S_IFCHR) {
    	                return 1;
    	            }
    	            break;
    	        case 'b':
    	            if ((entry_data->st_mode & S_IFMT) == S_IFBLK) {
    	                return 1;
    	            }
    	            break;
    	        case 'p':
    	            if ((entry_data->st_mode & S_IFMT) == S_IFIFO) {
    	                return 1;
    	            }
    	            break;
    	        case 'l':
    	            if ((entry_data->st_mode & S_IFMT) == S_IFLNK) {
    	                return 1;
    	            }
    	            break;
    	        case 's':
    	            if ((entry_data->st_mode & S_IFMT) == S_IFSOCK) {
    	                return 1;
    	            }
    	            break;
    	        case 'f':
    	            if ((entry_data->st_mode & S_IFMT) == S_IFREG) {
    	                return 1;
    	            }
    	            break;
    	        default:
    	            error(EXIT_FAILURE, errno, "Unknown type -type: %c", type_scan);
    }

    	    return 0;

    	 }
    static void do_path(char *parms) {
        printf("path Gesucht wird nach: %s", parms);
    }
    static void do_print(char *parms) {
        printf("print Gesucht wird nach: %s", parms);
    }
    static void do_ls(char *parms) {
        printf("ls Gesucht wird nach: %s", parms);
    }
