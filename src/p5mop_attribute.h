#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_ATTR_H
#define MOP_ATTR_H

#define ATTR_NAME_SLOT       newSVpv("$!name", 6)
#define ATTR_ASSOC_META_SLOT newSVpv("$!associated_meta", 17)

/* *****************************************************
 * Constructors
 * ***************************************************** */

#define newMopMaV(name) THX_newMopMaV(aTHX_ name)

SV* THX_newMopMaV(pTHX_ SV* name);

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