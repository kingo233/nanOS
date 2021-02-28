/*************************************************************************
	> File Name: defs.h
	> Author: jiangnan
	> Mail: kingo233@outlook.com
	> Created Time: 2021年02月28日 星期日 14时14分06秒
 ************************************************************************/
/*
    这个文件定义的一些东西：
    NULL
    和整数有关的一些类型
    两个变成n的倍数的宏函数
    获得成员在某一类型中地址偏移量的宏函数
    获得成员所在的结构体的宏函数
*/
#ifndef _DEFS_H
#define _DEFS_H
//NULL的定义
#ifndef NULL
#define NULL (void*)0

//GNU C扩展语法的简写定义
#define __always_inline inline __attribute__((always_inline))
#define __noinline __attribute__((noinline))
#define __noreturn __attribute__((noreturn))

//类型的定义

typedef bool int;
/* Explicitly-sized versions of integer types */
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

/* *
 * Pointers and addresses are 32 bits long.
 * We use pointer types to represent addresses,
 * uintptr_t to represent the numerical values of addresses.
 * */
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;
typedef uint32_t size_t;


//把a变成n的倍数，向下取整
#define ROUNDDOWN(a,n) ({\
    uint32_t __a = (uint32_t)(a);\
    (typeof(a)) (__a - (__a) % (n));\
})

//把a变成n的倍数，向上取整
#define ROUNDUP(a,n) ({\
    uint32_t __a = (uint32_t) (a);\
    uint32_t __n = (uint32_t) (n);\
    (typedef)(a) (ROUNDDOWN(__a + __n - 1),__n)\
})

//获取member在type中的偏移量
#define OFFSET(type,member) ( (uint32_t) (& ((type*)0) -> member) )

/* *
 * to_struct - get the struct from a ptr
 * @ptr:    a struct pointer of member
 * @type:   the type of the struct this is embedded in
 * @member: the name of the member within the struct
 * */
#define to_struct(ptr, type, member)                               \
    ( (type *)( (char*)(ptr) - OFFSET(type, member) ) )
#endif
#endif
