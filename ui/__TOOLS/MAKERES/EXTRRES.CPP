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
#include <list.h>
#include "BaseDef.h"


/*
    ex)
    // ¡¡—e •A·¡ÈáŸi  e—i ˜… 4¤a·¡?Ÿi ‹¡¥¥¸÷i?”a.

    c:\...> type game.rc

    #resname resource.res
    #compress on

    sprite1.spr
    tile1.til
    statge1.map
    etc.dat
    .
    .


    c:\...> makeres game[.rc]
*/

#pragma pack(push, 1)

typedef struct {
    CHAR    Header[20];
    LONG    Version;
    LONG    FileCount;
    LONG    ResourceSize;
    CHAR    Reserved[64-(20+4+4+4)];
} RESOURCE_HEADER;

typedef struct {
    LONG    Index;              /* Resource Index         */
    CHAR    FileName[40];       /* ÆÄÀÏ ¸í                */
    LONG    Offset;             /* Çì´õ¸¦ Á¦¿ÜÇÑ ¿É¼Â     */
    LONG    IsCompress;         /* ¾ÐÃà ¿©ºÎ              */
    LONG    FileSize;           /* È­ÀÏ »çÀÌÁî            */
    LONG    FileOrgSize;        /* ¾ÐÃàÇÏ±â ÀüÀÇ »çÀÌÁî   */
    atDWORD   ID;
	//	  atCHAR	Reserved[64 - (4+40+4+4+4+4+4)];  // 64¸¦ Ã¤¿ö¼­ »°À½.
} FILE_HEADER;


class FILE_NAME {
  public:
    INT     Index;
    CHAR    FileName[256];
    CHAR    FullName[256];
    BOOL    CompressMode;

    FILE_NAME( void )
    {
      Index = 0;
      memset( FileName, 0, 256 );
      memset( FullName, 0, 256 );
      CompressMode = TRUE;
    }

    ~FILE_NAME() {}

    FILE_NAME &operator = ( const FILE_NAME &x )
    {
      Index = x.Index;
      memcpy( FileName, x.FileName, 256 );
      memcpy( FullName, x.FullName, 256 );
      CompressMode = x.CompressMode;

      return( *this );
    }

    friend bool operator == ( const FILE_NAME &x, const FILE_NAME &y )
    {
      return( !strcmp( x.FileName, y.FileName ) );
    }

    friend bool operator < ( const FILE_NAME &x, const FILE_NAME &y )
    {
      return( strcmp( x.FileName, y.FileName ) < 0 );
    }

    friend bool operator > ( const FILE_NAME &x, const FILE_NAME &y )
    {
      return( strcmp( x.FileName, y.FileName ) > 0 );
    }
};

#pragma pack(pop)



extern ULONG Encode( CHAR **DesData, CHAR *SrcData, ULONG DataSize );


char RESName[128] = "RESOURCE.RES";
char RCName[128] = "RESOURCE.RC";
BOOL CompressMode = TRUE;



/*****************************************
  ¯a?Ÿ·· ¹Á¶ ‰·¤‚·¡a '\n'·i ¹Aˆá?”a.
******************************************/
void trim( char *str )
{
    char *s = str;

    while( *s != '\0' && *s != '\n' && (*s == ' ' || *s == '\t') ) s++;

    if( s != str ){
      char *t = str;
      do
        *t++ = *s++;
      while( *s != '\0' );
    }

    int l = strlen( str );
    char *t = s = str + l - 1;

    while( s != str && (*s == '\n' || *s == ' ' || *s == '\t') ) s--;

    if( s != t ) *(s + 1) = '\0';
}


/***************************************
  ?¯a¡w·i ¹A¶A? ?·©·¡Ÿq·i ™A´á…”a.
****************************************/
void RemoveFileExt( char *filename )
{
    char *s = filename;

    while( *s != '\0' && *s != '\n' ) s ++;
    s --;

    for( int i = 0; i < 3+1; i ++ ){
      if( *s == '.' ){
        *s = '\0';
        break;
      }
      s --;
    }
}


/*******************************
  ?·©¡w· ÑÂ¸w¸aŸi ¹Aˆá?”a.
********************************/
void ExtractFileName( char *filename )
{
    int index = 0;
    int lastindex = -1;

    while( filename[index] != '\0' ){
      if( filename[index] == '\\' || filename[index] == '/' )
        lastindex = index;

      index ++;
    }

    if( lastindex >= 0 ){
      lastindex ++;

      for( int i = 0; i <= index - lastindex; i ++ )
        filename[i] = filename[i + lastindex];
    }
}


/***********************
  RC ?·©·i ¦…¬â?”a.
************************/
BOOL GenerateRCFile( char *RC_FileName, list<FILE_NAME> &fl )
{
    FILE_NAME fn;
    char filename[256];
    FILE *Ifp, *fp;


    // RC ?·©·i µe”a.
    if( (Ifp = fopen( RC_FileName, "rt" )) == NULL ){
      puts( "RC File open error!\n" );
      return( FALSE );
    }

    int index = 0;

    while( fgets( filename, 255, Ifp ) != NULL ){
      if( filename[0] == '#' ){
        BOOL SetErr = TRUE;
        if( !strncmp( &filename[1], "compress", 8 ) ){
          char tempstr[256];

          strcpy( tempstr, &filename[10] );
          trim( tempstr );
          if( !strncmp( tempstr, "on", 2 ) ){
            CompressMode = TRUE;
            SetErr = FALSE;
          } else if( !strncmp( tempstr, "off", 3 ) ){
            CompressMode = FALSE;
            SetErr = FALSE;
          }
        } else if( !strncmp( &filename[1], "resname", 7 ) ){
          SetErr = FALSE;
          strcpy( RESName, &filename[9] );
          trim( RESName );
        }

        if( SetErr ){
          RemoveCR( filename );
          printf( "Declation Syntex Error in \"%s\"!\n", filename );
          return( FALSE );
        }
      } else {
        trim( filename );

        if( filename[0] == '\0' || filename[0] == '\n' || filename[0] == ';' )
          continue;

        RemoveComent( filename );

        // CR Å¡—aŸi ´ô´…”a.
        if( (fp = fopen( filename, "rb" )) == NULL ){
          printf( "File Read Error for %s!\n", filename );
          return( FALSE );
        }
        fclose( fp );

        fn.Index = index ++;

        strupr( filename );
        strcpy( fn.FullName, filename );

        // ?·© ·¡Ÿq·i ´è“e”a.
        ExtractFileName( filename );
        strcpy( fn.FileName, filename );

        fn.CompressMode = CompressMode;

        fl.insert( fl.end(), fn );
      }
    }

    return( TRUE );
}


/***********************
  Ÿ¡?¯aŸi ¬—?? …”a.
************************/
BOOL ExtractResource( char *RES_FileName, list<FILE_NAME> &fl )
{
    int i;
    list<FILE_NAME>::iterator fn;
    RESOURCE_HEADER rh = { "Amos Resource File;", 0x100, 0, 0, };
                         // 12345678901234567890
    FILE_HEADER *fh, *fh_back;
    FILE    *Ofp, *fp;
    char    *src, *des, *des2;
    INT     filesize, index = 0;
    INT     tOrgSize, tCompSize;


    // RES ?·©·i µe”a.
    if( (Ofp = fopen( RES_FileName, "wb" )) == NULL ){
      puts( "Cannot Create RES File!\n" );
      return( FALSE );
    }

    // ·©”e ?·© ?”áŸi °á ‰·ˆe·i  ae?”a.
    rh.FileCount = fl.size();
    fwrite( &rh, sizeof(RESOURCE_HEADER), 1, Ofp );

    // •A·¡Èá ?”á ¡A¡¡Ÿ¡ ?”w
    fh = fh_back = new FILE_HEADER[ rh.FileCount ];

    // ·©”e •A·¡Èá ?”áŸi °á ‰·ˆe·i  ae?”a.
    fwrite( fh, sizeof(FILE_HEADER) * rh.FileCount, 1, Ofp );

    tOrgSize = tCompSize = 0;

    // ?·©¡w· Àõ¤å¼ µIŸ¡ å?
    fn = fl.begin();

    // ˆb •A·¡ÈáŸi ³e”a.
    for( i = 0; i < rh.FileCount; i ++ ){
        if( (fp = fopen( (*fn).FullName, "rb" )) == NULL ){
          printf( "File Read Error for %s!\n", (*fn).FullName );
          return( FALSE );
        }

        memset( fh->FileName, 0, 40/*8+1+3*/ );
        strncpy( fh->FileName, (*fn).FileName, 40/*8+1+3*/ );

        fh->Offset = index;
        filesize = filelength( fileno(fp) );
        fh->FileOrgSize = filesize;
        src = new char[ filesize ];
        fread( src, filesize, 1, fp );
        fclose( fp );

        if( (*fn).CompressMode ){
          // ?·©·i ´sÂ‚?”a.
          filesize = Encode( &des, src, (ULONG)filesize );
        } else {
          filesize = 0;
        }

        fh->Index = (*fn).Index;

        //  e´¢ ´sÂ‚·¡ –A´ö”a¡e...
        if( filesize > 0 ){
          free( src );
          fh->FileSize = filesize;
          fh->IsCompress = TRUE;
        } else {
          des = src;
          fh->FileSize = fh->FileOrgSize;
          fh->IsCompress = FALSE;
        }

        // 4 ¤a·¡? ¸÷i·i ¶á?...
        filesize = fh->FileSize;
        if( filesize % 4 > 0 ) filesize += 4 - filesize % 4;

        tOrgSize += fh->FileOrgSize;
        tCompSize += filesize;

        des2 = (char *)malloc( filesize );
        memset( des2, 0, filesize );
        memcpy( des2, des, fh->FileSize );

        fwrite( des2, filesize, 1, Ofp );
        free( des );
        free( des2 );

        index += filesize;

        // ÑÁ¡eµA ‰Á¸÷ ??
        printf( "%-12s : %9ld ==> %9ld (Org %9ld) ... %5.1f%%\n",
                                                    (*fn).FileName, fh->FileOrgSize,
                                                    filesize, fh->FileSize,
                                                    (float)fh->FileSize / fh->FileOrgSize * 100 );

        fh ++;
        fn ++;
    }

    // ¹A”¡ –E ?·© ?”áŸi ³e”a.
    rh.ResourceSize = sizeof(RESOURCE_HEADER) + tCompSize;
    fseek( Ofp, 0, SEEK_SET );
    fwrite( &rh, sizeof(RESOURCE_HEADER), 1, Ofp );

    // •A·¡Èá ?”áŸi ”a? ³e”a.
    fwrite( fh_back, sizeof(FILE_HEADER) * rh.FileCount, 1, Ofp );

    // •A·¡Èá ?”á ¡A¡¡Ÿ¡ ?¹A
    free( fh_back );

    fclose( Ofp );

    printf( "\nOriginal Size : %d ==> Compress Size : %d\n", tOrgSize, tCompSize );

    return( TRUE );
}


int main( int argc, char *argv[] )
{
    BOOL result;
    char infile[128];
    list<FILE_NAME> fl;


    if( argc < 2 ){
        puts( "RESOURCE EXTRACTOR v0.5\n" );
        puts( "Uses : EXTRRES resource[.res]\n" );
        exit( 1 );
    }

    strcpy( infile, argv[1] );
    if( !strstr( infile, "." ) ) strcat( infile, ".RES" );

    strcpy( RESName, argv[1] );
    RemoveFileExt( RESName );
    strcat( RCName, ".RC" );

    result = GenerateRCFile( infile, fl ) ;
    if( !result ) return( 1 );

    fl.sort();
    result = ExtractResource( RESName, fl );
    if( !result ) return( 1 );
    
    return( 0 );
}

