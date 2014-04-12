#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_CLASS_H
#define MOP_CLASS_H

#ifndef MOP_CLASS_H
#include "p5mop.h"
#endif

/* *****************************************************
 * Constructors
 * ***************************************************** */

#define newMopMCV(name) THX_newMopMCV(aTHX_ name)

SV* THX_newMopMCV(pTHX_ SV* name);

/* *****************************************************
 * Accessors
 * ***************************************************** */

#define MopMCV_get_name(metaclass) THX_MopMCV_get_name(aTHX_ metaclass)
#define MopMCV_get_version(metaclass) THX_MopMCV_get_version(aTHX_ metaclass)
#define MopMCV_get_authority(metaclass) THX_MopMCV_get_authority(aTHX_ metaclass)

SV* THX_MopMCV_get_name(pTHX_ SV* metaclass);
SV* THX_MopMCV_get_version(pTHX_ SV* metaclass);
SV* THX_MopMCV_get_authority(pTHX_ SV* metaclass);

#endif /* MOP_CLASS_H */