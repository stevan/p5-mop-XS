#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_ATTR_H
#define MOP_ATTR_H

#ifndef MOP_H
#include "p5mop.h"
#endif

/* *****************************************************
 * Constructors
 * ***************************************************** */

#define newMopMaV(name) THX_newMopMaV(aTHX_ name)

SV* THX_newMopMaV(pTHX_ SV* name);

/* *****************************************************
 * Accessors
 * ***************************************************** */

#define MopMaV_get_name(meta_attr) THX_MopMaV_get_name(aTHX_ meta_attr)

SV* THX_MopMaV_get_name(pTHX_ SV* meta_attr);

/* *****************************************************
 * Methods
 * ***************************************************** */

#endif /* MOP_ATTR_H */