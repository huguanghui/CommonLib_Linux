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

#endif