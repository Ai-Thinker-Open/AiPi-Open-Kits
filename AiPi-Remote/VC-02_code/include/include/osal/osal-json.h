/*
  Copyright (c) 2009-2017 Dave Gamble and OsalJson contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef OSAL_OSAL_JSON_H_
#define OSAL_OSAL_JSON_H_

#include "osal/osal.h"
#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__WINDOWS__) && \
    (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__

/* When compiling for windows, we specify a specific calling convention to avoid
issues where we are being called from a project with a different default calling
convention.  For windows you have 3 define options:

CJSON_HIDE_SYMBOLS - Define this in the case where you don't want to ever
dllexport symbols CJSON_EXPORT_SYMBOLS - Define this on library build when you
want to dllexport symbols (default) CJSON_IMPORT_SYMBOLS - Define this if you
want to dllimport symbol

For *nix builds that support visibility attribute, you can define similar
behavior by

setting default visibility to hidden by adding
-fvisibility=hidden (for gcc)
or
-xldscope=hidden (for sun cc)
to CFLAGS

then using the CJSON_API_VISIBILITY flag to "export" the same symbols the way
CJSON_EXPORT_SYMBOLS does

*/

#define CJSON_CDECL __cdecl
#define CJSON_STDCALL __stdcall

/* export symbols by default, this is necessary for copy pasting the C and
 * header file */
#if !defined(CJSON_HIDE_SYMBOLS) && !defined(CJSON_IMPORT_SYMBOLS) && \
    !defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_EXPORT_SYMBOLS
#endif

#if defined(CJSON_HIDE_SYMBOLS)
#define CJSON_PUBLIC(type) type CJSON_STDCALL
#elif defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_PUBLIC(type) __declspec(dllexport) type CJSON_STDCALL
#elif defined(CJSON_IMPORT_SYMBOLS)
#define CJSON_PUBLIC(type) __declspec(dllimport) type CJSON_STDCALL
#endif
#else /* !__WINDOWS__ */
#define CJSON_CDECL
#define CJSON_STDCALL

/* AIK 打包的OsalJson不需要export到外部 */
#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined(__SUNPRO_C)) && \
    defined(CJSON_API_VISIBILITY)
#define CJSON_PUBLIC(type) __attribute__((visibility("hidden"))) type
#else
#define CJSON_PUBLIC(type) type
#endif
#endif

/* project version */
#define CJSON_VERSION_MAJOR 1
#define CJSON_VERSION_MINOR 7
#define CJSON_VERSION_PATCH 12

#include <stddef.h>

/* OsalJson Types: */
#define OsalJson_Invalid (0)
#define OsalJson_False (1 << 0)
#define OsalJson_True (1 << 1)
#define OsalJson_NULL (1 << 2)
#define OsalJson_Number (1 << 3)
#define OsalJson_String (1 << 4)
#define OsalJson_Array (1 << 5)
#define OsalJson_Object (1 << 6)
#define OsalJson_Raw (1 << 7) /* raw json */

#define OsalJson_IsReference 256
#define OsalJson_StringIsConst 512

/* The OsalJson structure: */
typedef struct OsalJson {
  /* next/prev allow you to walk array/object chains. Alternatively, use
   * GetArraySize/GetArrayItem/GetObjectItem */
  struct OsalJson* next;
  struct OsalJson* prev;
  /* An array or object item will have a child pointer pointing to a chain of
   * the items in the array/object. */
  struct OsalJson* child;

  /* The type of the item, as above. */
  int type;

  /* The item's string, if type==OsalJson_String  and type == OsalJson_Raw */
  char* valuestring;
  /* writing to valueint is DEPRECATED, use OsalJson_SetNumberValue instead */
  int valueint;
  /* The item's number, if type==OsalJson_Number */
  double valuedouble;

  /* The item's name string, if this item is the child of, or is in the list of
   * subitems of an object. */
  char* string;
} OsalJson;

typedef struct OsalJson_Hooks {
  /* malloc/free are CDECL on Windows regardless of the default calling
   * convention of the compiler, so ensure the hooks allow passing those
   * functions directly. */
  void*(CJSON_CDECL* malloc_fn)(size_t sz);
  void(CJSON_CDECL* free_fn)(void* ptr);
} OsalJson_Hooks;

typedef int OsalJson_bool;

/* Limits how deeply nested arrays/objects can be before OsalJson rejects to
 * parse them. This is to prevent stack overflows. */
#ifndef CJSON_NESTING_LIMIT
#define CJSON_NESTING_LIMIT 1000
#endif

/* returns the version of OsalJson as a string */
CJSON_PUBLIC(const char*) OsalJson_Version(void);

/* Supply malloc, realloc and free functions to OsalJson */
CJSON_PUBLIC(void) OsalJson_InitHooks(OsalJson_Hooks* hooks);

/* Memory Management: the caller is always responsible to free the results from
 * all variants of OsalJson_Parse (with OsalJson_Delete) and OsalJson_Print
 * (with stdlib free, OsalJson_Hooks.free_fn, or OsalJson_free as appropriate).
 * The exception is OsalJson_PrintPreallocated, where the caller has full
 * responsibility of the buffer. */
/* Supply a block of JSON, and this returns a OsalJson object you can
 * interrogate.
 */
CJSON_PUBLIC(OsalJson*) OsalJson_Parse(const char* value);
/* ParseWithOpts allows you to require (and check) that the JSON is null
 * terminated, and to retrieve the pointer to the final byte parsed. */
/* If you supply a ptr in return_parse_end and parsing fails, then
 * return_parse_end will contain a pointer to the error so will match
 * OsalJson_GetErrorPtr(). */
CJSON_PUBLIC(OsalJson*)
OsalJson_ParseWithOpts(const char* value, const char** return_parse_end,
                       OsalJson_bool require_null_terminated);

/* Render a OsalJson entity to text for transfer/storage. */
CJSON_PUBLIC(char*) OsalJson_Print(const OsalJson* item);
/* Render a OsalJson entity to text for transfer/storage without any formatting.
 */
CJSON_PUBLIC(char*) OsalJson_PrintUnformatted(const OsalJson* item);
/* Render a OsalJson entity to text using a buffered strategy. prebuffer is a
 * guess at the final size. guessing well reduces reallocation. fmt=0 gives
 * unformatted, =1 gives formatted */
CJSON_PUBLIC(char*)
OsalJson_PrintBuffered(const OsalJson* item, int prebuffer, OsalJson_bool fmt);
/* Render a OsalJson entity to text using a buffer already allocated in memory
 * with given length. Returns 1 on success and 0 on failure. */
/* NOTE: OsalJson is not always 100% accurate in estimating how much memory it
 * will use, so to be safe allocate 5 bytes more than you actually need */
CJSON_PUBLIC(OsalJson_bool)
OsalJson_PrintPreallocated(OsalJson* item, char* buffer, const int length,
                           const OsalJson_bool format);
/* Delete a OsalJson entity and all subentities. */
CJSON_PUBLIC(void) OsalJson_Delete(OsalJson* item);

/* Returns the number of items in an array (or object). */
CJSON_PUBLIC(int) OsalJson_GetArraySize(const OsalJson* array);
/* Retrieve item number "index" from array "array". Returns NULL if
 * unsuccessful. */
CJSON_PUBLIC(OsalJson*) OsalJson_GetArrayItem(const OsalJson* array, int index);
/* Get item "string" from object. Case insensitive. */
CJSON_PUBLIC(OsalJson*)
OsalJson_GetObjectItem(const OsalJson* const object, const char* const string);
CJSON_PUBLIC(OsalJson*)
OsalJson_GetObjectItemCaseSensitive(const OsalJson* const object,
                                    const char* const string);
CJSON_PUBLIC(OsalJson_bool)
OsalJson_HasObjectItem(const OsalJson* object, const char* string);
/* For analysing failed parses. This returns a pointer to the parse error.
 * You'll probably need to look a few chars back to make sense of it. Defined
 * when OsalJson_Parse() returns 0. 0 when OsalJson_Parse() succeeds. */
CJSON_PUBLIC(const char*) OsalJson_GetErrorPtr(void);

/* Check if the item is a string and return its valuestring */
CJSON_PUBLIC(char*) OsalJson_GetStringValue(const OsalJson* const item);

/* These functions check the type of an item */
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsInvalid(const OsalJson* const item);
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsFalse(const OsalJson* const item);
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsTrue(const OsalJson* const item);
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsBool(const OsalJson* const item);
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsNull(const OsalJson* const item);
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsNumber(const OsalJson* const item);
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsString(const OsalJson* const item);
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsArray(const OsalJson* const item);
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsObject(const OsalJson* const item);
CJSON_PUBLIC(OsalJson_bool) OsalJson_IsRaw(const OsalJson* const item);

/* These calls create a OsalJson item of the appropriate type. */
CJSON_PUBLIC(OsalJson*) OsalJson_CreateNull(void);
CJSON_PUBLIC(OsalJson*) OsalJson_CreateTrue(void);
CJSON_PUBLIC(OsalJson*) OsalJson_CreateFalse(void);
CJSON_PUBLIC(OsalJson*) OsalJson_CreateBool(OsalJson_bool boolean);
CJSON_PUBLIC(OsalJson*) OsalJson_CreateNumber(double num);
CJSON_PUBLIC(OsalJson*) OsalJson_CreateString(const char* string);
/* raw json */
CJSON_PUBLIC(OsalJson*) OsalJson_CreateRaw(const char* raw);
CJSON_PUBLIC(OsalJson*) OsalJson_CreateArray(void);
CJSON_PUBLIC(OsalJson*) OsalJson_CreateObject(void);

/* Create a string where valuestring references a string so
 * it will not be freed by OsalJson_Delete */
CJSON_PUBLIC(OsalJson*) OsalJson_CreateStringReference(const char* string);
/* Create an object/array that only references it's elements so
 * they will not be freed by OsalJson_Delete */
CJSON_PUBLIC(OsalJson*) OsalJson_CreateObjectReference(const OsalJson* child);
CJSON_PUBLIC(OsalJson*) OsalJson_CreateArrayReference(const OsalJson* child);

/* These utilities create an Array of count items.
 * The parameter count cannot be greater than the number of elements in the
 * number array, otherwise array access will be out of bounds.*/
CJSON_PUBLIC(OsalJson*) OsalJson_CreateIntArray(const int* numbers, int count);
CJSON_PUBLIC(OsalJson*)
OsalJson_CreateFloatArray(const float* numbers, int count);
CJSON_PUBLIC(OsalJson*)
OsalJson_CreateDoubleArray(const double* numbers, int count);
CJSON_PUBLIC(OsalJson*)
OsalJson_CreateStringArray(const char* const* strings, int count);

/* Append item to the specified array/object. */
CJSON_PUBLIC(void) OsalJson_AddItemToArray(OsalJson* array, OsalJson* item);
CJSON_PUBLIC(void)
OsalJson_AddItemToObject(OsalJson* object, const char* string, OsalJson* item);
/* Use this when string is definitely const (i.e. a literal, or as good as), and
 * will definitely survive the OsalJson object. WARNING: When this function was
 * used, make sure to always check that (item->type & OsalJson_StringIsConst) is
 * zero before writing to `item->string` */
CJSON_PUBLIC(void)
OsalJson_AddItemToObjectCS(OsalJson* object, const char* string,
                           OsalJson* item);
/* Append reference to item to the specified array/object. Use this when you
 * want to add an existing OsalJson to a new OsalJson, but don't want to corrupt
 * your existing OsalJson. */
CJSON_PUBLIC(void)
OsalJson_AddItemReferenceToArray(OsalJson* array, OsalJson* item);
CJSON_PUBLIC(void)
OsalJson_AddItemReferenceToObject(OsalJson* object, const char* string,
                                  OsalJson* item);

/* Remove/Detach items from Arrays/Objects. */
CJSON_PUBLIC(OsalJson*)
OsalJson_DetachItemViaPointer(OsalJson* parent, OsalJson* const item);
CJSON_PUBLIC(OsalJson*)
OsalJson_DetachItemFromArray(OsalJson* array, int which);
CJSON_PUBLIC(void) OsalJson_DeleteItemFromArray(OsalJson* array, int which);
CJSON_PUBLIC(OsalJson*)
OsalJson_DetachItemFromObject(OsalJson* object, const char* string);
CJSON_PUBLIC(OsalJson*)
OsalJson_DetachItemFromObjectCaseSensitive(OsalJson* object,
                                           const char* string);
CJSON_PUBLIC(void)
OsalJson_DeleteItemFromObject(OsalJson* object, const char* string);
CJSON_PUBLIC(void)
OsalJson_DeleteItemFromObjectCaseSensitive(OsalJson* object,
                                           const char* string);

/* Update array items. */
CJSON_PUBLIC(void)
OsalJson_InsertItemInArray(
    OsalJson* array, int which,
    OsalJson* newitem); /* Shifts pre-existing items to the right. */
CJSON_PUBLIC(OsalJson_bool)
OsalJson_ReplaceItemViaPointer(OsalJson* const parent, OsalJson* const item,
                               OsalJson* replacement);
CJSON_PUBLIC(void)
OsalJson_ReplaceItemInArray(OsalJson* array, int which, OsalJson* newitem);
CJSON_PUBLIC(void)
OsalJson_ReplaceItemInObject(OsalJson* object, const char* string,
                             OsalJson* newitem);
CJSON_PUBLIC(void)
OsalJson_ReplaceItemInObjectCaseSensitive(OsalJson* object, const char* string,
                                          OsalJson* newitem);

/* Duplicate a OsalJson item */
CJSON_PUBLIC(OsalJson*)
OsalJson_Duplicate(const OsalJson* item, OsalJson_bool recurse);
/* Duplicate will create a new, identical OsalJson item to the one you pass, in
 * new memory that will need to be released. With recurse!=0, it will duplicate
 * any children connected to the item. The item->next and ->prev pointers are
 * always zero on return from Duplicate.
 */
/* Recursively compare two OsalJson items for equality. If either a or b is NULL
 * or invalid, they will be considered unequal. case_sensitive determines if
 * object keys are treated case sensitive (1) or case insensitive (0) */
CJSON_PUBLIC(OsalJson_bool)
OsalJson_Compare(const OsalJson* const a, const OsalJson* const b,
                 const OsalJson_bool case_sensitive);

/* Minify a strings, remove blank characters(such as ' ', '\t', '\r', '\n') from
 * strings. The input pointer json cannot point to a read-only address area,
 * such as a string constant,
 * but should point to a readable and writable adress area. */
CJSON_PUBLIC(void) OsalJson_Minify(char* json);

/* Helper functions for creating and adding items to an object at the same time.
 * They return the added item or NULL on failure. */
CJSON_PUBLIC(OsalJson*)
OsalJson_AddNullToObject(OsalJson* const object, const char* const name);
CJSON_PUBLIC(OsalJson*)
OsalJson_AddTrueToObject(OsalJson* const object, const char* const name);
CJSON_PUBLIC(OsalJson*)
OsalJson_AddFalseToObject(OsalJson* const object, const char* const name);
CJSON_PUBLIC(OsalJson*)
OsalJson_AddBoolToObject(OsalJson* const object, const char* const name,
                         const OsalJson_bool boolean);
CJSON_PUBLIC(OsalJson*)
OsalJson_AddNumberToObject(OsalJson* const object, const char* const name,
                           const double number);
CJSON_PUBLIC(OsalJson*)
OsalJson_AddStringToObject(OsalJson* const object, const char* const name,
                           const char* const string);
CJSON_PUBLIC(OsalJson*)
OsalJson_AddRawToObject(OsalJson* const object, const char* const name,
                        const char* const raw);
CJSON_PUBLIC(OsalJson*)
OsalJson_AddObjectToObject(OsalJson* const object, const char* const name);
CJSON_PUBLIC(OsalJson*)
OsalJson_AddArrayToObject(OsalJson* const object, const char* const name);

/* When assigning an integer value, it needs to be propagated to valuedouble
 * too. */
#define OsalJson_SetIntValue(object, number) \
  ((object) ? (object)->valueint = (object)->valuedouble = (number) : (number))
/* helper for the OsalJson_SetNumberValue macro */
CJSON_PUBLIC(double) OsalJson_SetNumberHelper(OsalJson* object, double number);
#define OsalJson_SetNumberValue(object, number)                        \
  ((object != NULL) ? OsalJson_SetNumberHelper(object, (double)number) \
                    : (number))

/* Macro for iterating over an array or object */
#define OsalJson_ArrayForEach(element, array)                              \
  for (element = (array != NULL) ? (array)->child : NULL; element != NULL; \
       element = element->next)

/* malloc/free objects using the malloc/free functions that have been set with
 * OsalJson_InitHooks */
CJSON_PUBLIC(void*) OsalJson_malloc(size_t size);
CJSON_PUBLIC(void) OsalJson_free(void* object);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_JSON_H_
