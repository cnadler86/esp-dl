#ifndef _MP_COMPAT_H_
#define _MP_COMPAT_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdlib.h>

// Basic MicroPython types
typedef int32_t mp_int_t;
typedef uint32_t mp_uint_t;
typedef long mp_off_t;

// Object representation
typedef void *mp_obj_t;
typedef const void *mp_const_obj_t;

// Print enumerations - must define enum before forward declaration
enum _mp_print_kind_t {
    PRINT_STR = 0,
    PRINT_REPR,
    PRINT_EXC,
    PRINT_JSON
};
typedef enum _mp_print_kind_t mp_print_kind_t;

// ROM map element structure
struct _mp_rom_map_elem_t {
    mp_obj_t key;
    mp_obj_t value;
};
typedef struct _mp_rom_map_elem_t mp_rom_map_elem_t;

// Forward declarations for types
typedef struct _mp_obj_type_t mp_obj_type_t;
typedef struct _mp_obj_base_t mp_obj_base_t;
typedef struct _mp_obj_dict_t mp_obj_dict_t;
typedef struct _mp_map_t mp_map_t;
typedef struct _mp_print_t mp_print_t;

// Basic object structure
struct _mp_obj_base_t {
    const mp_obj_type_t *type;
};

// Print struct
struct _mp_print_t {
    void *data;
    void (*print_strn)(void*, const char*, size_t);
};

// Object type structure (simplified version)
struct _mp_obj_type_t {
    mp_obj_base_t base;
    const char *name;
    void (*print)(const mp_print_t *print, mp_obj_t self, mp_print_kind_t kind);
    mp_obj_dict_t *locals_dict;
};

// Map structure
struct _mp_map_t {
    size_t all_keys_are_qstrs : 1;
    size_t is_fixed : 1;
    size_t is_ordered : 1;
    size_t used : (8 * sizeof(size_t) - 3);
    size_t alloc;
    void *table;
};

// Dict structure (simplified)
struct _mp_obj_dict_t {
    mp_obj_base_t base;
    mp_map_t map;
};

// Import stat values
#define MP_IMPORT_STAT_FILE 1
#define MP_IMPORT_STAT_DIR 2
#define MP_IMPORT_STAT_NO_EXIST 0
typedef int mp_import_stat_t;

// Define stub constants for QSTRs
#define MP_QSTR_readinto 0
#define MP_QSTR_seek 0
#define MP_QSTR_tell 0
#define MP_QSTR_write 0
#define MP_QSTR_close 0
#define MP_QSTR_mp_file 0
#define MP_QSTR___del__ 0

// Define stub macros for ROM
#define MP_ROM_QSTR(qstr) ((mp_obj_t)(qstr))
#define MP_ROM_PTR(ptr) ((mp_obj_t)(ptr))

// Define stub macros for object creation
#define m_new_obj(type) ((type*)malloc(sizeof(type)))
#define MP_OBJ_TO_PTR(o) ((void*)(o))
#define MP_OBJ_NEW_SMALL_INT(i) ((mp_obj_t)(i))

// Stub macros for defining functions and dictionaries
#define MP_DEFINE_CONST_FUN_OBJ_1(obj_name, fun_name) \
    const mp_obj_t obj_name = (mp_obj_t)&fun_name

#define MP_DEFINE_CONST_DICT(dict_name, dict_table) \
    const mp_obj_dict_t dict_name = { \
        .base = {NULL}, \
        .map = { \
            .all_keys_are_qstrs = 1, \
            .is_fixed = 1, \
            .is_ordered = 0, \
            .used = 0, \
            .alloc = 0, \
            .table = (void*)dict_table \
        } \
    }

#define MP_DEFINE_CONST_OBJ_TYPE(type_name, qstr, flags, print_method, print_func, locals_dict_name, locals_dict) \
    const mp_obj_type_t type_name = { \
        .base = {NULL}, \
        .name = (qstr), \
        .print = print_func, \
        .locals_dict = (mp_obj_dict_t*)(locals_dict), \
    }

// Common MicroPython constants
extern const mp_obj_t mp_const_none;
extern const mp_obj_t mp_const_empty_map;
extern const mp_obj_t MP_OBJ_NULL;
extern const mp_obj_type_t mp_type_type;

// Function to print debug info
void mp_printf(const mp_print_t *print, const char *fmt, ...);

// Helper functions
mp_obj_t mp_obj_new_str(const char *data, size_t len);
mp_obj_t mp_obj_new_bytearray_by_ref(size_t n, void *items);
mp_obj_t mp_load_attr(mp_obj_t base, unsigned int attr);
mp_obj_t mp_call_function_0(mp_obj_t fun);
mp_obj_t mp_call_function_1(mp_obj_t fun, mp_obj_t arg1);
mp_obj_t mp_call_function_2(mp_obj_t fun, mp_obj_t arg1, mp_obj_t arg2);
mp_int_t mp_obj_get_int(mp_obj_t o);
mp_import_stat_t mp_vfs_import_stat(const char *path);
mp_obj_t mp_vfs_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs);

#endif // _MP_COMPAT_H_