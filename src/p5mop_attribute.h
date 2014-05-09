#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_ATTR_H
#define MOP_ATTR_H

/* *****************************************************
 * Constructors
 * ***************************************************** */

#define newMopMaV(name, name_len) THX_newMopMaV(aTHX_ name, name_len)

SV* THX_newMopMaV(pTHX_ const char* name, STRLEN name_len);

/* *****************************************************
 * Accessors
 * ***************************************************** */

#define MopMaV_get_name(meta_attr) THX_MopMaV_get_name(aTHX_ meta_attr)
#define MopMaV_get_key_name(meta_attr) THX_MopMaV_get_key_name(aTHX_ meta_attr)

SV* THX_MopMaV_get_name(pTHX_ SV* meta_attr);
SV* THX_MopMaV_get_key_name(pTHX_ SV* meta_attr);

// associated metaclass

#define MopMaV_get_associated_class(meta_attr) THX_MopMaV_get_associated_class(aTHX_ meta_attr)
#define MopMaV_set_associated_class(meta_attr, metaclass) THX_MopMaV_set_associated_class(aTHX_ meta_attr, metaclass)

SV*  THX_MopMaV_get_associated_class(pTHX_ SV* meta_attr);
void THX_MopMaV_set_associated_class(pTHX_ SV* meta_attr, SV* metaclass);

/* *****************************************************
 * Methods
 * ***************************************************** */

#endif /* MOP_ATTR_H */