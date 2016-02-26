#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// If you are compiling this on a non cygwin environment it's bin should
// be placed on scripts directory and being so userlist relative path
// should be "../dat/userlist.db" and "../dat/temp.db".
// Under cygwin the executable should be placed on Darkbot's root dir
// due to the necessity of cygwin dynamic library.  
// So... comment next 2 defines and uncomment the following 2 if you are
// not compiling convertdb with cygwin. Also change what is commented
// on "dat/userlist.db dat/backup.db" a couple lines bellow.
// There is already a compiled bin on /scripts for unixes
#define USERFILE	"dat/userlist.db"
#define TMPFILE     "dat/temp.db"
// #define USERFILE        "../dat/userlist.db"
// #define TMPFILE     "../dat/temp.db"



char *crypt (const char *key, const char *salt);

int main(int argc, char *argv[]){    FILE *in, *out;
    char *ptr = NULL;
	char *salt = "8fei3k";
    char channel[1024] = { 0 };
    char uh[1024] = { 0 };
    char level[10] = { 0 };
    char joins[10] = { 0 };
    char pass[1024] = { 0 };
    char setinfo[1024] = { 0 };

    printf("\nDarkbot USERLIST.DB < 7f0 conversion utility.\n");
    printf("IMPORTANT :: READ THIS!\n\n");
    printf("This UTILITY is for converting *OLD* (pre 7f0) userlist's\n");
    printf("to the new encrypted format.\n\n");
    printf("Use this utility ONLY if:\n");
    printf(" - You have copied your pre 7f0 userlist.db in to the dat directory\n\n");
    printf("DO NOT use this utility if:\n");
    printf(" - You have created a brand new darkbot, and have no users\n\n");

    if(argc < 2)
    {
        printf("Syntax: %s -convert\n\n", argv[0]);
        return 0;
    }

    if(argc > 1)
    {
        if(strcmp(argv[1], "-convert") != 0)
        {
            printf("Syntax: %s -convert\n\n", argv[0]);
            return 0;
        }
    }
    
    printf("Converting ...\n");

    if((in = fopen(USERFILE, "r")) == NULL)
    {
        printf("\nCan't open %s for reading!\n", USERFILE);
        return EXIT_FAILURE;
    }    if((out = fopen(TMPFILE, "wt")) == NULL)
    {
        printf("\nCan't open %s for writing!\n", TMPFILE);
        return EXIT_FAILURE;
    }

// Comment next line and uncomment the following one if you are
// not compiling convertdb with cygwin.
    system("cp dat/userlist.db dat/backup.db");    while(!feof(in))
//    system("cp ../dat/userlist.db ../dat/backup.db");    while(!feof(in))

    {
	    fscanf(in, "%s %s %s %s %s %[^\n]s", channel, uh, level, joins, pass, setinfo);

        if ((ptr = crypt (pass, salt)) == NULL)
        {
            printf("\ncrypt() error\n");
            return EXIT_FAILURE;
        }
        if(!feof(in))
            fprintf(out, "%s %s %s %s %s %s\n", channel, uh, level, joins, ptr, setinfo);
    }
    fclose(in);
    fclose(out);

    remove(USERFILE);
    rename(TMPFILE, USERFILE);

    printf("Conversion Complete.\n");
    
    return EXIT_SUCCESS;}
