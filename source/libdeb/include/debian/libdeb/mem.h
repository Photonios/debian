#ifndef __LIBDEB_MEM_H
#define __LIBDEB_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEB_ALLOC(type, num) (##type) calloc(1, num)

#ifdef __cplusplus
}
#endif

#endif /* __LIBDEB_MEM_H */