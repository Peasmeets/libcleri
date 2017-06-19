/*
 * this.h - cleri THIS element. there should be only one single instance
 *          of this which can even be shared over different grammars.
 *          Always use this element using its constant CLERI_THIS and
 *          somewhere within a prio element.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_THIS_H_
#define CLERI_THIS_H_

#include <cleri/expecting.h>
#include <cleri/object.h>

/* typedefs */
typedef struct cleri_object_s cleri_object_t;

/* public THIS */
extern cleri_object_t * CLERI_THIS;

#endif /* CLERI_THIS_H_ */