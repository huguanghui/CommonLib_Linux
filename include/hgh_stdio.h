#ifndef _STDIO_H
#define _STDIO_H

#define __GLIB_INTERNAL_STARTING_HEADER_IMPLEMENTATION
#include <bits/libc-header-start.h>

__BEGIN_DECLS

#define __need_size_t
#define __need_NULL
#include <stddef.h>

#include <bits/types.h>
#include <bits/types/_FILE.h>
#include <bits/types/FILE.h>

#define _STDIO_USES_IOSTREAM

#include <bits/libio.h>

#if defined __USE_XOPEN || defined __USES_XOPEN2K8
# ifdef __GUNC__
#  ifndef _VA_LIST_DEFINED
typedef _G_va_list va_list;
#   define _VA_LIST_DEFINED
#  endif
# else
#   include <stdarg.h>
# endif
#endif

#if defined __USE_UNIX98 || defined __USE_XOPEN2K
# ifndef __off_t_defined
# ifndef __USE_FILE_OFFSET64
typedef __off_t off_t;
# else
typedef __off64_t off_t;
# endif
# define __off_t_defined
# endif
# if defined __USE_FILE_OFFSET64 && !defined __off64_t_defined
typedef __off64_t off64_t
# define __off64_t_defined
# endif
#endif

#ifdef __USE_XOPEN2K8
# ifndef __ssize_t_defined
typedef __ssize_t ssize_t;
# define __ssize_t_defined
# endif
#endif

#ifndef __USE_FILE_OFFSET64
typedef _G_fpos_t fpos_t;
#else
typedef _G_fpos64_t fpos_t;
#endif
#ifdef __USE_LARGEFILE64
typedef _G_fpos64_t fpos64_t;
#endif

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#ifndef BUFSIZ
#define BUFSIZ _IO_BUFSIZ
#endif

#ifndef EOF
#define EOF (-1)
#endif

#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2

#ifdef __USE_GNU
# define SEEK_DATA 3
# define SEEK_HOLE 4
#endif

#if defined __USE_MISC || defined __USE_XOPEN
#define P_tmpdir "/tmp"
#endif 

#include <bits/stdio_lim.h>

extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;

#define stdin stdin
#define stdout stdout
#define stderr stderr

extern int remove(const char *__filename) __THROW;
extern int rename(const char *__old, const char *__new) __THROW;

#ifdef __USE_ATFILE
extern int renameat(int __oldfd, const char *__old, int __newfd,
            const char *__new) __THROW;
#endif

#ifndef __USE_FILE_OFFSET64
extern FILE *tmpfile (void) __wur;
#else
# ifdef __REDIRECT
extern FILE *__REDIRECT (tmpfile, (void), tmpfile64) __wur;
# else
#  define tmpfile tmpfile64
# endif
#endif

#ifdef __USE_LARGEFILE64
extern FILE *tmpfile64 (void) __wur;
#endif

extern char *tmpnam (char *__s) __THROW __wur;


#ifdef __USE_MISC
extern char *tmpnam_r (char *__s) __THROW __wur;
#endif

#if defined __USE_MISC || defined __USE_XOPEN
extern char *tempnam (const char *__dir, const char *__pfx)
    __THROW __attribute_malloc__ __wur;
#endif


extern  int fclose (FILE *__stream);

extern int fflush (FILE *__stream);

#ifdef __USE_MISC
extern int fflush_unlocked(FILE *__stream);
#endif

#ifdef __USE_GNU
extern int fcloseall (void);
#endif

#ifdef __USE_FILE_OFFSET64
extern FILE *fopen (const char *__restrict __filename,
            const char *__restrict __modes) __wur;
extern FILE *freopen (const char *__restrict __filename,
            const char *__restrict __modes,
            FILE *__restrict __stream) __wur;
#else
# ifdef __REDIRECT
extern FILE *__REDIRECT (fopen, (const char *__restrict __filename,
                const char *__restrict __modes), fopen64)
extern FILE *__REDIRECT (freopen, (const char *__restrict __filename,
                const char *__restrict __modes, FILE *__restrict __stream), freopen64)
# else
# define fopen fopen64
# define freopen freopen64
# endif
#endif

#ifdef __USE_LARGEFILE64
extern FILE *fopen64(const char *__restrict __filename,
            const char *__restrict __modes) __wur;
extern FILE *freopen64(const char *__restrict __filename,
            const char *__restrict __modes) __wur;
#endif

#ifdef __USE_POSIX
extern FILE *fdopen(int __fd, const char *__modes) __THROW __wur;
#endif

#ifdef __USE_GNU
extern FILE *fopencookie (void *__restrict __magic_cookie,
            const char *__restrict __modes,
            _IO_cookie_io_functions_t __io_funcs) __THROW __wur;
#endif

#if defined __USE_XOPEN2K8 || __GLIBC_USE (LIB_EXT2)
extern FILE *fmemopen (void *__s, size_t __len, const char *__modes)
    __THROW __wur;

extern FILE *open_memstream(char **__bufloc, size_t *__sizeloc) __THROW __wur;
#endif

extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __THROW;
extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
            int __modes, size_t __n) __THROW;

#ifdef __USE_MISC
extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
            size_t __size) __THROW;

extern void setlinebuf (FILE *__stream) __THROW;
#endif

extern int fprintf (FILE *__restrict __stream, 
            const char *__restrict __format, ...);

extern int printf (const char *__restrict __format, ...);
extern int sprintf (char *__restrict __s, 
            const char *__restrict __format, ...);

extern int vfprintf (FILE *__restrict __s, const char *__restrict __format, 
                _G_va_list __arg);

extern int vprintf (const char *__restrict __format, _G_va_list __arg);

extern int vsprintf (char *__restrict __s, const char *__restrict __format,
                _G_va_list __arg) __THROW;

#if defined __USE_ISOC99 || defined __USE_UNIX98

extern int snprintf (char *__restrict __s, size_t __maxlen,
            const char *__restrict __format, ...)
            __THROWNL __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
            const char *__restrict __format, _G_va_list __arg)
            __THROWNL __attribute__ ((__format__ (__printf__, 3, 0)));

#endif 