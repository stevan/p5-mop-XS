#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_CLASS_H
#define MOP_CLASS_H

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

// superclass

#define MopMcV_get_superclass(metaclass) THX_MopMcV_get_superclass(aTHX_ metaclass)
#define MopMcV_set_superclass(metaclass, superclass) THX_MopMcV_set_superclass(aTHX_ metaclass, superclass)

SV*  THX_MopMcV_get_superclass(pTHX_ SV* metaclass);
void THX_MopMcV_set_superclass(pTHX_ SV* metaclass, SV* superclass);

// methods

#define MopMcV_has_method(metaclass, name) THX_MopMcV_has_method(aTHX_ metaclass, name)
#define MopMcV_get_method(metaclass, name) THX_MopMcV_get_method(aTHX_ metaclass, name)
#define MopMcV_add_method(metaclass, name, code) THX_MopMcV_add_method(aTHX_ metaclass, name, code)

bool THX_MopMcV_has_method(pTHX_ SV* metaclass, SV* name);
SV*  THX_MopMcV_get_method(pTHX_ SV* metaclass, SV* name);
void THX_MopMcV_add_method(pTHX_ SV* metaclass, SV* name, SV* code);

/* *****************************************************
 * Methods
 * ***************************************************** */

#define MopMcV_construct_instance(metaclass, repr) THX_MopMcV_construct_instance(aTHX_ metaclass, repr)

SV* THX_MopMcV_construct_instance(pTHX_ SV* metaclass, SV* repr);

#endif /* MOP_CLASS_H */