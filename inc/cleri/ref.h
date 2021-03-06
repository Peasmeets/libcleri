/*
 * ref.h - cleri ref element
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2017, Transceptor Technology
 *
 * changes
 *  - initial version, 20-06-2017
 *
 */
#ifndef CLERI_REF_H_
#define CLERI_REF_H_

#include <stddef.h>
#include <inttypes.h>
#include <cleri/cleri.h>

/* typedefs */
typedef struct cleri_s cleri_t;

/* public functions */
cleri_t * cleri_ref(void);
void cleri_ref_set(cleri_t * ref, cleri_t * cl_obj);

#endif /* CLERI_REF_H_ */