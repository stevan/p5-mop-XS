#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_CLASS_H
#define MOP_CLASS_H

#define CLASS_ATTRIBUTE_SLOT newSVpv("$!attributes", 12)

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

// attributes

#define MopMcV_has_attribute(metaclass, name) THX_MopMcV_has_attribute(aTHX_ metaclass, name)
#define MopMcV_get_attribute(metaclass, name) THX_MopMcV_get_attribute(aTHX_ metaclass, name)
#define MopMcV_add_attribute(metaclass, attribute) THX_MopMcV_add_attribute(aTHX_ metaclass, attribute)

bool THX_MopMcV_has_attribute(pTHX_ SV* metaclass, SV* name);
SV*  THX_MopMcV_get_attribute(pTHX_ SV* metaclass, SV* name);
void THX_MopMcV_add_attribute(pTHX_ SV* metaclass, SV* attribute);

// methods

#define MopMcV_has_method(metaclass, name) THX_MopMcV_has_method(aTHX_ metaclass, name)
#define MopMcV_get_method(metaclass, name) THX_MopMcV_get_method(aTHX_ metaclass, name)
#define MopMcV_upgrade_method(metaclass, code) THX_MopMcV_upgrade_method(aTHX_ metaclass, code)
#define MopMcV_add_method(metaclass, name, code) THX_MopMcV_add_method(aTHX_ metaclass, name, code)

bool THX_MopMcV_has_method(pTHX_ SV* metaclass, SV* name);
SV*  THX_MopMcV_get_method(pTHX_ SV* metaclass, SV* name);
SV*  THX_MopMcV_upgrade_method(pTHX_ SV* metaclass, SV* code);
void THX_MopMcV_add_method(pTHX_ SV* metaclass, SV* name, SV* code);

/* *****************************************************
 * Methods
 * ***************************************************** */

#define MopMcV_construct_instance(metaclass, repr) THX_MopMcV_construct_instance(aTHX_ metaclass, repr)

SV* THX_MopMcV_construct_instance(pTHX_ SV* metaclass, SV* repr);

#endif /* MOP_CLASS_H */