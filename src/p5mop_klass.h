#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_KLASS_H
#define MOP_KLASS_H

#ifndef MOP_KLASS_H
#include "p5mop.h"
#endif

/* *****************************************************
 * Constructors
 * ***************************************************** */

#define newMopMKV(name) THX_newMopMKV(aTHX_ name)

SV* THX_newMopMKV(pTHX_ SV* name);

/* *****************************************************
 * Accessors
 * ***************************************************** */

#define MopMKV_get_name(metaclass) THX_MopMKV_get_name(aTHX_ metaclass)
#define MopMKV_get_version(metaclass) THX_MopMKV_get_version(aTHX_ metaclass)
#define MopMKV_get_authority(metaclass) THX_MopMKV_get_authority(aTHX_ metaclass)

SV* THX_MopMKV_get_name(pTHX_ SV* metaclass);
SV* THX_MopMKV_get_version(pTHX_ SV* metaclass);
SV* THX_MopMKV_get_authority(pTHX_ SV* metaclass);

#endif /* MOP_KLASS_H */