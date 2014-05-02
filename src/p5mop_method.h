#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_METHOD_H
#define MOP_METHOD_H

/* *****************************************************
 * Constructors
 * ***************************************************** */

#define newMopMmV(code, steal_stash) THX_newMopMmV(aTHX_ code, steal_stash)

SV* THX_newMopMmV(pTHX_ SV* code, SV* steal_stash);

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