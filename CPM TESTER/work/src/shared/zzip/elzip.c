#include <windows.h>
#include <stdio.h>
#include <zzip/zzip.h>
#include "elzip.h"

typedef ZZIP_DIR *  	(*zzip_dir_open_t)(zzip_char_t* filename, zzip_error_t * errcode_p); 
typedef ZZIP_DIR *  	(*zzip_dir_fdopen_t)(int fd, zzip_error_t * errcode_p);
typedef int             (*zzip_dir_read_t)(ZZIP_DIR * dir, ZZIP_DIRENT * dirent);
typedef int	  			(*zzip_dir_close_t)(ZZIP_DIR * dir);  

typedef ZZIP_FILE * 	(*zzip_file_open_t)(ZZIP_DIR * dir, zzip_char_t* name, int flags);  
typedef zzip_ssize_t	(*zzip_file_read_t)(ZZIP_FILE * fp, void* buf, zzip_size_t len); 
typedef int	 			(*zzip_close_t)(ZZIP_FILE * fp);     

typedef struct _SZZip
{
	/* zip archive */
	ZZIP_DIR *  	(*dir_open)(zzip_char_t* filename, zzip_error_t * errcode_p); 
	ZZIP_DIR *  	(*dir_fdopen)(int fd, zzip_error_t * errcode_p); 
	int             (*dir_read)(ZZIP_DIR * dir, ZZIP_DIRENT * dirent);  
	int	  			(*dir_close)(ZZIP_DIR * dir);
	
	/* files in archive */
	ZZIP_FILE * 	(*file_open)(ZZIP_DIR * dir, zzip_char_t* name, int flags);     
    zzip_ssize_t	(*file_read)(ZZIP_FILE * fp, void* buf, zzip_size_t len);
	int	 			(*file_close)(ZZIP_FILE * fp);  
	
	HINSTANCE          	_hinstLib;     
	
} SZZip, *SZZipPtr;

static SElExceptionPtr	zzip_new(SZZipPtr* pZZip);
static SElExceptionPtr	zzip_delete(SZZipPtr* pZZip);

static SElExceptionPtr DecompressFile(struct _SZip* me, const char* archive, const char* file, const char* output);

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elzip_new"
SElExceptionPtr	elzip_new(SZipPtr* pZip)
{
	SElExceptionPtr pexception = NULL;
	SZZipPtr		pzzip;	
	
	*pZip = (SZipPtr)calloc( 1, sizeof(SZip));
   	EXCCHECKALLOC( *pZip);
   
	EXCCHECK( zzip_new( &pzzip));
	(*pZip)->zzip = (void*)pzzip;
	
	(*pZip)->DecompressFile = DecompressFile;
	
Error:
   	EXCRETHROW( pexception); 
} /* elzip_new */	

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elzip_delete"
SElExceptionPtr	elzip_delete(SZipPtr* pZip)
{
	SElExceptionPtr pexception = NULL;    
	SZZipPtr		pzzip = NULL;
	
	if(pZip && *pZip)
	{
		pzzip = (SZZipPtr)(*pZip)->zzip;
		EXCCHECK( zzip_delete(&pzzip));
	
		free(*pZip);
		*pZip = NULL;
	}
	
Error:
	EXCRETHROW( pexception); 	
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "zzip_new"
static SElExceptionPtr	zzip_new(SZZipPtr* pZZip)
{
	SElExceptionPtr pexception = NULL;

	*pZZip = calloc( 1, sizeof(SZZip));
   	EXCCHECKALLOC( *pZZip);
	
	(*pZZip)->_hinstLib = LoadLibrary("zzip.dll");
	
	if((*pZZip)->_hinstLib)
	{
		(*pZZip)->dir_open = (zzip_dir_open_t)GetProcAddress((*pZZip)->_hinstLib, "zzip_dir_open"); 
		(*pZZip)->dir_fdopen = (zzip_dir_fdopen_t)GetProcAddress((*pZZip)->_hinstLib, "zzip_dir_fdopen"); 
		(*pZZip)->dir_read = (zzip_dir_read_t)GetProcAddress((*pZZip)->_hinstLib, "zzip_dir_read");  
		(*pZZip)->dir_close = (zzip_dir_close_t)GetProcAddress((*pZZip)->_hinstLib, "zzip_dir_close");
	
		(*pZZip)->file_open = (zzip_file_open_t)GetProcAddress((*pZZip)->_hinstLib, "zzip_file_open");  
		(*pZZip)->file_read = (zzip_file_read_t)GetProcAddress((*pZZip)->_hinstLib, "zzip_file_read");
		(*pZZip)->file_close = (zzip_close_t)GetProcAddress((*pZZip)->_hinstLib, "zzip_close"); 
	}
	else
	{
		EXCTHROW( -1, "zzip.dll and zlib1.dll do not exist in windows\\system32 directory!");
	}
	
Error:
   	EXCRETHROW( pexception); 
} /* zzip_new */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "zzip_delete"
static SElExceptionPtr	zzip_delete(SZZipPtr* pZZip)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;

	if((*pZZip)->_hinstLib)
	{
		EXCCHECKWIN( FreeLibrary((*pZZip)->_hinstLib)); 
		(*pZZip)->_hinstLib = NULL;
	}
	free(*pZZip);
	*pZZip = NULL;
	
Error:
   	EXCRETHROW( pexception); 
} /* zzip_delete */

#define FILE_BLOCK		128
/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "DecompressFile"
static SElExceptionPtr DecompressFile(
	struct _SZip* me, 
	const char* sarchive, 
	const char* sfile, 
	const char* soutput
)
{
	SElExceptionPtr	pexception = NULL;
	SZZipPtr		pzzip = (SZZipPtr)me->zzip;     
	FILE 			*outfile = NULL; 
	ZZIP_DIR 		*dir;
	ZZIP_DIRENT 	diren;
	ZZIP_FILE 		*file;
	
	if(pzzip->_hinstLib)
	{
		dir = pzzip->dir_open(sarchive, 0);
		if(dir)
		{
			file = pzzip->file_open(dir, sfile, 0);
			if(file)
			{
				char		buffer[FILE_BLOCK];
				int 		num_read = 0;
			
				outfile = fopen(soutput, "wb");
				
				while( (num_read = pzzip->file_read(file, buffer, FILE_BLOCK))>0)
				{
					fwrite(buffer, 1, num_read, outfile);
				}
			
				pzzip->file_close(file);
				fclose(outfile);  
			}
		
			pzzip->dir_close(dir);
		}
	}
		
Error:
   	EXCRETHROW( pexception);
}