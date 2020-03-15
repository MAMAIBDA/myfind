
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

#include <pwd.h>
#include <grp.h>

#define MAXLEN 256
#define NULLCHAR 1

#define MATCH 1
#define NO_MATCH 0

static void no_argv(int argc, char ** parms);
//do_entry(const char * entry_name, const char * const * parms);

static void do_entry(const char * entry_name, char ** parms);
static void do_dir(const char * dir_name, char ** parms);
static void do_name(const char *parms);
static int do_type(const char *parms, const struct stat *entry_data);
static void do_path(char *parms);
static void do_print(char *parms);
static void do_ls(char *parms);

static int do_username(struct stat entry_data, const char * param);
static int do_uid(struct stat entry_data, const char * param);
static int do_user(struct stat entry_data, const char * param);
static int do_nouser(struct stat entry_data);
static int do_groupname(struct stat entry_data, const char * param);
static int do_gid(struct stat entry_data, const char * param);
static int do_group(struct stat entry_data, const char * param);
static int do_nogroup(struct stat entry_data);


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
		 printf ("\ndefault directory\n ");
	parms[1]="."; //set default directory in case not specified
	parms[2]=NULL;} // set NULL as next argument to prevent going out of argv boundries

	else if(argc >= 2 ){
		if (*parms[1] == '-'){
			char *temp;
			int n=argc+2;
				 printf ("default directory ... %s sollte gegeben sein", parms[1]);
				 for (int i= n-2;i>=1; i--){ //change the position of the arguments to the right 
					 temp = parms[i];
					 parms[i+1]=temp;
				 }

				 parms[1]="."; // adding the default directory
				 parms[n]=NULL;}} // adding NULL to the end to avoid overflow

	//	printf("\n arguments are:");
	// for (int i=argc ; i>0; i--){
		// printf("\n argv[%d]: %s \n",i,parms[i]);
	// }

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
	    strcpy(buffer, parms[i]);
        if (*parms[i] == '-'){                                                                                                                                    
            for (int j = 0; j < 10; j++) {                                                                                                                        
                                                                                                                                        
                if ((strcmp(possible_entry[j], buffer)) == 0) {                                                                                                   
                    printf("Gefunden:%s!!\n", possible_entry[j]); //                                                                                              
                                                                                                                                                               
                if (j == 0) {                                                                                                                                      
                                                                                                                                                                  
               //    do_nogroup(parms[j + 1]);                                                                                                                      
                                                                                                                                                                  
                } else if (j == 1) {                                                                                                                               
                                                                                                                                                                  
                  //  do_group(parms[i + 1]);                                                                                                                       
                                                                                                                                                                  
                } else if (j == 2) {                                                                                                                               
                                                                                                                                                                  
               //     do_nouser(parms[i + 1]);                                                                                                                      
                                                                                                                                                                  
                } else if (j == 3) {                                                                                                                               
                                                                                                                                                                  
                 //   do_user(parms[i + 1]);                                                                                                                        
                                                                                                                                                                  
                } else if (j == 4) {                                                                                                                               
                    do_dir_flag=0;                                                                                            
                    do_name(parms[i + 1]);
		} else if (j == 5) {
                	do_type(parms[i+1],&entry_data); 
			i++;
                	continue;
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


static int do_username(struct stat entry_data, const char * param)
{
    errno = 0;
    const struct passwd *pwd_entry;

    pwd_entry = getpwnam(param);
    if (pwd_entry == NULL){

        if (errno!=0)
            error(0,errno, "username");// - oder nicht drucken wenn keins gefunden wird?
        return NO_MATCH;
    }
    else if(entry_data.st_uid == pwd_entry->pw_uid)
    {
        return MATCH;
    }

}




static int do_uid(struct stat entry_data, const char * param)
{
    const struct passwd *pwd_entry;
    int match;
    long int uid;
    char *pt;

    uid = strtol(param,&pt,0);

    if(pt != NULL) {
        printf("something went wrong!");
    }
    if ((pwd_entry = getpwuid(uid)) != NULL) {
        if (pwd_entry->pw_uid == entry_data.st_uid) {
            return MATCH;
        }
    }
    else {
        if (errno != 0) {
            error(0, errno, "uid");

        }

        return NO_MATCH;
    }
}



static int do_user(struct stat entry_data, const char * param) {

    if (do_username(entry_data, param) || do_uid(entry_data, param) == 1) {

        return MATCH;
    } else {
       return NO_MATCH;
    }


}


static int do_nouser(struct stat entry_data)
{
    errno = 0;
  const struct passwd * pwd_etnry;

    pwd_etnry = getpwuid(entry_data.st_uid);
    if (pwd_etnry == NULL)
    {
        if (errno!=0)
        {
            error(0, errno, "nouser");
        }
        else
        {
            return MATCH;
        }
    }

    return NO_MATCH;
}




static int do_groupname(struct stat entry_data, const char * param)
{
    errno = 0;
    const struct passwd *grp_entry;

    grp_entry = getgrnam(param);
    if (grp_entry == NULL){

        if (errno!=0)
            error(0,errno, "groupname");// - oder nicht drucken wenn keins gefunden wird?
        return NO_MATCH;
    }
    else if(entry_data.st_uid == grp_entry->pw_uid)
    {
        return MATCH;
    }

}




static int do_gid(struct stat entry_data, const char * param)
{
    const struct passwd *grp_entry;
    long int gid;
    char *pt;

    gid = strtol(param,&pt,0);

    if(pt != NULL) {
        printf("something went wrong!");
    }
    if ((grp_entry = getgrgid(gid)) != NULL) {
        if (grp_entry->pw_uid == entry_data.st_uid) {
            return MATCH;
        }
    }
    else {
        if (errno != 0) {
            error(0, errno, "gid");

        }

        return NO_MATCH;
    }
}



static int do_group(struct stat entry_data, const char * param) {

    if (do_groupname(entry_data, param) || do_gid(entry_data, param) == 1) {

        return MATCH;
    } else {
        return NO_MATCH;
    }


}


static int do_nogroup(struct stat entry_data)
{
    errno = 0;
    const struct passwd * grp_etnry;

    grp_etnry = getgrgid(entry_data.st_uid);
    if (grp_etnry == NULL)
    {
        if (errno!=0)
        {
            error(0, errno, "nogroup");
        }
        else
        {
            return MATCH;
        }
    }
    return NO_MATCH;
}




