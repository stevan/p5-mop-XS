#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_CLASS_H
#define MOP_CLASS_H

#ifndef MOP_H
#include "p5mop.h"
#endif

/* *****************************************************
 * Constructors
 * ***************************************************** */

#define newMopMcV(name) THX_newMopMcV(aTHX_ name)

SV* THX_newMopMcV(pTHX_ SV* name);

/* *****************************************************
 * Accessors
 * ***************************************************** */

#define MopMcV_get_name(metaclass) THX_MopMcV_get_name(aTHX_ metaclass)
#define MopMcV_get_version(metaclass) THX_MopMcV_get_version(aTHX_ metaclass)
#define MopMcV_get_authority(metaclass) THX_MopMcV_get_authority(aTHX_ metaclass)

SV* THX_MopMcV_get_name(pTHX_ SV* metaclass);
SV* THX_MopMcV_get_version(pTHX_ SV* metaclass);
SV* THX_MopMcV_get_authority(pTHX_ SV* metaclass);

/* *****************************************************
 * Methods
 * ***************************************************** */

#define MopMcV_construct_instance(metaclass, repr) THX_MopMcV_construct_instance(aTHX_ metaclass, repr)

SV* THX_MopMcV_construct_instance(pTHX_ SV* metaclass, SV* repr);

#endif /* MOP_CLASS_H */