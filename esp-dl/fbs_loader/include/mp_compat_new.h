#ifndef _MP_COMPAT_H_
#define _MP_COMPAT_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdlib.h>

// Basic MicroPython types - matching ESP32 port definition
typedef int32_t mp_int_t;       // must be pointer size
typedef uint32_t mp_uint_t;     // must be pointer size  
typedef long mp_off_t;

// QSTR type - matching real MicroPython
typedef size_t qstr;

// Object representation - simplified to avoid casting issues
typedef void *mp_obj_t;
typedef const void *mp_const_obj_t;

// ROM object representation - simplified
typedef struct _mp_rom_obj_t {
    void *ptr;
} mp_rom_obj_t;

// Print function type
typedef void (*mp_print_strn_t)(void *data, const char *str, size_t len);

// Print struct - matching real MicroPython
typedef struct _mp_print_t {
    void *data;
    mp_print_strn_t print_strn;
} mp_print_t;

// Print enumerations - must define enum before forward declaration
typedef enum {
    PRINT_STR = 0,
    PRINT_REPR = 1,
    PRINT_EXC = 2,
    PRINT_JSON = 3
} mp_print_kind_t;

// Map element structure
typedef struct _mp_map_elem_t {
    mp_obj_t key;
    mp_obj_t value;
} mp_map_elem_t;

// ROM map element structure
typedef struct _mp_rom_map_elem_t {
    mp_obj_t key;
    mp_obj_t value;
} mp_rom_map_elem_t;

// Forward declarations for types
typedef struct _mp_obj_type_t mp_obj_type_t;
typedef struct _mp_obj_base_t mp_obj_base_t;
typedef struct _mp_obj_dict_t mp_obj_dict_t;
typedef struct _mp_map_t mp_map_t;

// Basic object structure
typedef struct _mp_obj_base_t {
    const mp_obj_type_t *type;
} mp_obj_base_t;

// Map structure - simplified but compatible
typedef struct _mp_map_t {
    size_t all_keys_are_qstrs : 1;
    size_t is_fixed : 1;
    size_t is_ordered : 1;
    size_t used : (8 * sizeof(size_t) - 3);
    size_t alloc;
    mp_map_elem_t *table;
} mp_map_t;

// Object type structure - simplified but compatible with existing code
typedef struct _mp_obj_type_t {
    mp_obj_base_t base;
    uint16_t flags;
    uint16_t name;          // qstr, not char*
    void (*print)(const mp_print_t *print, mp_obj_t self, mp_print_kind_t kind);
    mp_obj_dict_t *locals_dict;
} mp_obj_type_t;

// Dict structure
typedef struct _mp_obj_dict_t {
    mp_obj_base_t base;
    mp_map_t map;
} mp_obj_dict_t;

// Import stat values - matching builtin.h
typedef enum {
    MP_IMPORT_STAT_NO_EXIST = 0,
    MP_IMPORT_STAT_DIR = 1,
    MP_IMPORT_STAT_FILE = 2,
} mp_import_stat_t;

// Define stub constants for QSTRs - these should be proper qstr values
#define MP_QSTR_readinto 1
#define MP_QSTR_seek 2
#define MP_QSTR_tell 3
#define MP_QSTR_write 4
#define MP_QSTR_close 5
#define MP_QSTR_mp_file 6
#define MP_QSTR___del__ 7

// Object creation macros - more accurate to real MicroPython
#ifndef MP_OBJ_TO_PTR
#define MP_OBJ_TO_PTR(o) ((void *)(o))
#endif

#ifndef MP_OBJ_FROM_PTR
#define MP_OBJ_FROM_PTR(p) ((mp_obj_t)(p))
#endif

// Small integer creation - simplified version
#define MP_OBJ_NEW_SMALL_INT(small_int) ((mp_obj_t)((uintptr_t)(small_int)))

// QSTR creation macro
#define MP_OBJ_NEW_QSTR(qst) ((mp_obj_t)((uintptr_t)(qst)))

// ROM macros - simplified to avoid const issues
#define MP_ROM_QSTR(q) ((mp_obj_t)(q))
#define MP_ROM_PTR(p) ((mp_obj_t)(p))

// Memory allocation - use proper MicroPython allocation
void *m_malloc(size_t num_bytes);
void m_free(void *ptr);
#define m_new(type, num) ((type *)(m_malloc(sizeof(type) * (num))))
#define m_new_obj(type) (m_new(type, 1))
#define m_del(type, ptr, num) m_free(ptr)

// Type flags from real MicroPython
#define MP_TYPE_FLAG_INSTANCE_TYPE (0x0001)
#define MP_TYPE_FLAG_EQ_NOT_REFLEXIVE (0x0002)
#define MP_TYPE_FLAG_EQ_CHECKS_OTHER_TYPE (0x0004)
#define MP_TYPE_FLAG_EQ_HAS_NEQ_TEST (0x0008)
#define MP_TYPE_FLAG_BINDS_SELF (0x0010)
#define MP_TYPE_FLAG_BUILTIN_FUN (0x0020)
#define MP_TYPE_FLAG_ITER_IS_GETITER (0x0040)
#define MP_TYPE_FLAG_ITER_IS_ITERNEXT (0x0080)
#define MP_TYPE_FLAG_ITER_IS_STREAM (0x0100)

// Object sentinel values
#define MP_OBJ_NULL             ((mp_obj_t)0)
#define MP_OBJ_STOP_ITERATION   ((mp_obj_t)0)
#define MP_OBJ_SENTINEL         ((mp_obj_t)4)

// Function object macros - simplified for compilation
#define MP_DEFINE_CONST_FUN_OBJ_1(obj_name, fun_name) \
    const mp_obj_t obj_name = (mp_obj_t)&fun_name

#define MP_DEFINE_CONST_DICT(dict_name, dict_table) \
    const mp_obj_dict_t dict_name = { \
        .base = {&mp_type_dict}, \
        .map = { \
            .all_keys_are_qstrs = 1, \
            .is_fixed = 1, \
            .is_ordered = 0, \
            .used = 0, \
            .alloc = 0, \
            .table = (mp_map_elem_t*)dict_table \
        } \
    }

// Type definition macro - simplified
#define MP_DEFINE_CONST_OBJ_TYPE(type_name, qstr_name, flags, ...) \
    const mp_obj_type_t type_name = { \
        .base = {&mp_type_type}, \
        .flags = flags, \
        .name = qstr_name, \
        .print = NULL, \
        .locals_dict = NULL \
    }

// Common MicroPython constants
extern const mp_obj_t mp_const_none;
extern const mp_obj_t mp_const_empty_map;
extern const mp_obj_type_t mp_type_type;
extern const mp_obj_type_t mp_type_dict;

// Function declarations with correct signatures from runtime.h
int mp_printf(const mp_print_t *print, const char *fmt, ...);

// Object creation functions - correct signatures
mp_obj_t mp_obj_new_str(const char *data, size_t len);
mp_obj_t mp_obj_new_bytearray_by_ref(size_t n, void *items);

// Attribute and function call operations - corrected signatures
mp_obj_t mp_load_attr(mp_obj_t base, qstr attr);  // Uses qstr, not unsigned int
mp_obj_t mp_call_function_0(mp_obj_t fun);
mp_obj_t mp_call_function_1(mp_obj_t fun, mp_obj_t arg);
mp_obj_t mp_call_function_2(mp_obj_t fun, mp_obj_t arg1, mp_obj_t arg2);

// Type conversion functions
mp_int_t mp_obj_get_int(mp_obj_t o);

// VFS functions - correct signatures
mp_import_stat_t mp_vfs_import_stat(const char *path);
mp_obj_t mp_vfs_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs);

#endif // _MP_COMPAT_H_
