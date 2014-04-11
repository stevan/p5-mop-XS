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

#define newMopMCV(name, parent) THX_newMopMCV(aTHX_ name, parent)

SV* THX_newMopMCV(pTHX_ SV* name, HV* parent);

/* *****************************************************
 * Accessors
 * ***************************************************** */

#define MopMCV_get_name(metaclass) THX_MopMCV_get_name(aTHX_ metaclass)

SV* THX_MopMCV_get_name(pTHX_ SV* metaclass);
//SV* THX_MopMCV_get_version(pTHX_ SV* metaclass);
//SV* THX_MopMCV_get_authority(pTHX_ SV* metaclass);

#endif /* MOP_CLASS_H */