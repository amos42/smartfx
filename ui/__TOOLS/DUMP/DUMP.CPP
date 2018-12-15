/*###########################################################################
-----------------------------------------------------------------------------

                                dump.cpp
                         (* =================== *)

 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : Borland C/C++ 5.0
 Assembler         : 
 Dependent Modules :
 Description       : dump function definition
 
-----------------------------------------------------------------------------
###########################################################################*/
#include <stdio.h>


void main( int argc, char *argv[] )
{
	FILE *src, *dst;
	unsigned char a;
	int c=0;
	int d;
	long size;

  if( argc < 3 ){
    printf( "Usege : dump <src.bin> <des.txt>\n" );
    return;
  }
  
	src = fopen( argv[1], "rb" );
	if (src == NULL ){
	  printf ("src error!\n" );
	  return;
	}  

	dst = fopen( argv[2], "wt" );
	if (dst == NULL ){
	  printf ("dst error!\n" );
	  return;
	}

	fseek( src, 0L, SEEK_END );
	size = ftell(src);
	fseek( src, 0, SEEK_SET );

	fprintf ( dst, "unsigned char data[%ld] = { \n    ", size );
	
  for( int i=0; i < size; i++ ){
		fread(&a, 1, 1, src);
		fprintf ( dst, "0x%02X, ", a );

    if( c++ > 16 ){
			c = 0;
			fprintf ( dst, "\n    " );
		}
	}
	fprintf ( dst, "\n};\n" );

	fclose(dst);
	fclose(src);
}
