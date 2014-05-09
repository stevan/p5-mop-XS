#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_CLASS_H
#define MOP_CLASS_H

/* *****************************************************
 * Constructors
 * ***************************************************** */

#define newMopMcV(name, name_len) THX_newMopMcV(aTHX_ name, name_len)

SV* THX_newMopMcV(pTHX_ const char* name, STRLEN name_len);

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

#define MopMcV_has_method(metaclass, name, name_len) THX_MopMcV_has_method(aTHX_ metaclass, name, name_len)
#define MopMcV_get_method(metaclass, name, name_len) THX_MopMcV_get_method(aTHX_ metaclass, name, name_len)
#define MopMcV_upgrade_method(metaclass, code) THX_MopMcV_upgrade_method(aTHX_ metaclass, code)
#define MopMcV_add_method(metaclass, name, name_len, code) THX_MopMcV_add_method(aTHX_ metaclass, name, name_len, code)

bool THX_MopMcV_has_method(pTHX_ SV* metaclass, const char* name, STRLEN name_len);
SV*  THX_MopMcV_get_method(pTHX_ SV* metaclass, const char* name, STRLEN name_len);
SV*  THX_MopMcV_upgrade_method(pTHX_ SV* metaclass, SV* code);
void THX_MopMcV_add_method(pTHX_ SV* metaclass, const char* name, STRLEN name_len, SV* code);

/* *****************************************************
 * Methods
 * ***************************************************** */

#define MopMcV_construct_instance(metaclass, repr) THX_MopMcV_construct_instance(aTHX_ metaclass, repr)

SV* THX_MopMcV_construct_instance(pTHX_ SV* metaclass, SV* repr);

#endif /* MOP_CLASS_H */