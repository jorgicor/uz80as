/* config.h for uz80as Windows build */
#ifndef CONFIG_H
#define CONFIG_H

/* Package name and version */
#define PACKAGE "uz80as"
#define PACKAGE_NAME "uz80as"
#define PACKAGE_STRING "uz80as 1.0"
#define PACKAGE_VERSION "1.0"
#define VERSION "1.0"

/* System type */
#define HAVE_WINDOWS 1

/* Headers */
#define HAVE_ASSERT_H 1
#define HAVE_CTYPE_H 1
#define HAVE_STDARG_H 1
#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1

/* Functions */
#define HAVE_MALLOC 1
#define HAVE_REALLOC 1
#define HAVE_FOPEN 1
#define HAVE_FPRINTF 1
#define HAVE_VFPRINTF 1

/* Define to 1 if you have gettext support */
#define HAVE_GETTEXT 0

/* If gettext is not available, provide a fallback */
#ifndef HAVE_GETTEXT
#define _(String) (String)
#define N_(String) (String)
#endif

#define COPYRIGHT_YEARS "2024"
#define PACKAGE_BUGREPORT "jorge.giner@hotmail.com"
#define PACKAGE_URL "https://jorgicor.niobe.org/uz80as"

#endif /* CONFIG_H */
