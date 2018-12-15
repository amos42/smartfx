/*###########################################################################
-----------------------------------------------------------------------------

                                Makeres.cpp
                         (* =================== *)

 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : 
 Dependent Modules :
 Description       : make resource function definition
 
-----------------------------------------------------------------------------
###########################################################################*/

#include <stdio.h>
#include <malloc.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>

//#include "BaseDef.h"


int main( int argc, char *argv[] )
{
    char outfile[128];
    long curaddr, nextaddr;
    int blankchar;
    char str[128];
    char *data;
    long len;


    if( argc < 2 ){
        puts( "Binary Merge v0.5\r\n" );
        puts( "Uses : BinMerge -o output [-a addr1] [-b blank-char1] file1 [-a addr2] [-b blank-char1] file2 [[-a addr3] [-b blank-char3] file3 [...]]\r\n" );
        puts( "[*] Note : If [addr] is invalid, it will be ignored.\r\n" );
        return 1;
    }

    if( strcmp( argv[1], "-o" ) != 0 ) {
        puts( "Wrong parameter.\r\n" );
        return 2;
    } 

    strcpy( outfile, argv[2] );

    if(argc < 4) {
        puts( "Wrong parameter.\r\n" );
        return 2;
    }
    
    FILE *fp = fopen(outfile, "wb");
    
    blankchar = 0x00;
    nextaddr = 0;
    for(int i = 0; i < argc - 3; i++ ) {
      if(strcmp(argv[3 + i], "-a") == 0) {
        i++;
        strcpy( str, argv[3 + i] );
        if(strncmp(str, "0x", 2) == 0){
          nextaddr = strtoul( str + 2, NULL, 16 );
        } else {
          nextaddr = strtoul( str, NULL, 10 );
        }
        if(nextaddr <= curaddr) {
          nextaddr = 0;
        }        
      } else if(strcmp(argv[3 + i], "-b") == 0) {
        i++;
        strcpy( str, argv[3 + i] );
        if(strncmp(str, "0x", 2) == 0){
          blankchar = strtoul( str + 2, NULL, 16 );
        } else {
          blankchar = strtoul( str, NULL, 10 );
        }
        blankchar &= 0xFF;
      } else {
        if(nextaddr != 0) {
           len = nextaddr - curaddr;
           data = (char *)malloc(len);
           memset(data, (char)blankchar, len);
           fwrite(data, len, 1, fp);
           free(data);
           curaddr = nextaddr;
        }
        
        strcpy( str, argv[3 + i] );
        
        FILE *ifp = fopen(str, "rb");
        fseek(ifp, 0, SEEK_END);
        len = ftell(ifp);
        fseek(ifp, 0, SEEK_SET);
        data = (char *)malloc(len);
        fread(data, len, 1, ifp);
        fwrite(data, len, 1, fp);
        free( data );
        fclose(ifp);
        
        curaddr += len;
        nextaddr = 0;
      }
    }
    
    fclose( fp );

    return( 0 );
}

