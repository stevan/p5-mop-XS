#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_METHOD_H
#define MOP_METHOD_H

/* *****************************************************
 * Flags
 * ***************************************************** */

#define MopMmVf_STEAL_STASH 0x01

/* *****************************************************
 * Constructors
 * ***************************************************** */

#define newMopMmV(code, flags) THX_newMopMmV(aTHX_ code, flags)

SV* THX_newMopMmV(pTHX_ SV* code, U32 flags);

/* *****************************************************
 * Installers
 * ***************************************************** */

#define MopMmV_assign_to_stash(metamethod, gv, stash) THX_MopMmV_assign_to_stash(aTHX_ metamethod, gv, stash)

void THX_MopMmV_assign_to_stash(pTHX_ SV* metamethod, GV* gv, HV* stash);

/* *****************************************************
 * Accessors
 * ***************************************************** */

#define MopMmV_get_name(metamethod) THX_MopMmV_get_name(aTHX_ metamethod)
#define MopMmV_get_associated_class(metamethod) THX_MopMmV_get_associated_class(aTHX_ metamethod)

SV* THX_MopMmV_get_name(pTHX_ SV* metamethod);
SV* THX_MopMmV_get_associated_class(pTHX_ SV* metamethod);

#endif /* MOP_METHOD_H */