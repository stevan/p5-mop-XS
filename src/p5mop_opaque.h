#include "EXTERN.h"
#include "perl.h"

#ifndef MOP_OPAQUE_H
#define MOP_OPAQUE_H

/* *****************************************************
 * MopOV (O{paque,bject} Value) struct
 * ***************************************************** */

typedef struct {
    I32* id;
    HV*  slots;
    HV*  callbacks;
} MopOV;

// constructor

#define newMopOV(rv) THX_newMopOV(aTHX_ rv)
#define newMopOVsv() THX_newMopOV(aTHX_ newRV_noinc(newSV(0)))
#define newMopOVhv() THX_newMopOV(aTHX_ newRV_noinc((SV*) newHV()))
#define newMopOVav() THX_newMopOV(aTHX_ newRV_noinc((SV*) newAV()))

SV* THX_newMopOV(pTHX_ SV* rv);

// destructor

#define freeMopOV(opaque) THX_freeMopOV(aTHX_ opaque)

void THX_freeMopOV(pTHX_ MopOV* opaque);

// Slot access ...

#define MopOV_get_at_slot(rv, slot_name, slot_name_len) THX_MopOV_get_at_slot(aTHX_ rv, slot_name, slot_name_len)
#define MopOV_set_at_slot(rv, slot_name, slot_name_len, slot_value) THX_MopOV_set_at_slot(aTHX_ rv, slot_name, slot_name_len, slot_value)
#define MopOV_has_at_slot(rv, slot_name, slot_name_len) THX_MopOV_has_at_slot(aTHX_ rv, slot_name, slot_name_len)

SV*  THX_MopOV_get_at_slot(pTHX_ SV* rv, const char* slot_name, STRLEN slot_name_len);
void THX_MopOV_set_at_slot(pTHX_ SV* rv, const char* slot_name, STRLEN slot_name_len, SV* slot_value);
bool THX_MopOV_has_at_slot(pTHX_ SV* rv, const char* slot_name, STRLEN slot_name_len);

// Events ...

#define MopOV_has_events(rv) THX_MopOV_has_events(aTHX_ rv)
#define MopOV_bind_event(rv, event_name, callback) THX_MopOV_bind_event(aTHX_ rv, event_name, callback)
#define MopOV_unbind_event(rv, event_name, callback) THX_MopOV_unbind_event(aTHX_ rv, event_name, callback)
#define MopOV_fire_event(rv, event_name, args, args_len) THX_MopOV_fire_event(aTHX_ rv, event_name, args, args_len)

SV*  THX_MopOV_has_events(pTHX_ SV* rv);
void THX_MopOV_bind_event(pTHX_ SV* rv, SV* event_name, SV* callback);
void THX_MopOV_unbind_event(pTHX_ SV* rv, SV* event_name, SV* callback);
void THX_MopOV_fire_event(pTHX_ SV* rv, SV* event_name, SV** args, I32 args_len);


/* *****************************************************
 * General Utilities
 * ***************************************************** */

MopOV* SVrv_to_MopOV(SV* rv);
bool   isSVrv_a_MopOV(SV* rv);

#endif /* MOP_OPAQUE_H */