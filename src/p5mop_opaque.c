#include "p5mop_opaque.h"

/* *****************************************************
 * predeclare some internal functions
 * ***************************************************** */

static AV* fetch_events_by_name (HV* callbacks, SV* event_name);
static I32* new_uuid();

// magic destructor ...
static int mg_freeMopOV(pTHX_ SV *sv, MAGIC *mg);
static MGVTBL MopOV_vtbl = {
    NULL,         /* get   */
    NULL,         /* set   */
    NULL,         /* len   */
    NULL,         /* clear */
    mg_freeMopOV, /* free  */
    NULL,         /* copy  */
    NULL,         /* dup   */
    NULL          /* local */
};

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopOV(pTHX_ SV* rv) {
    assert(rv != NULL);

    if (SvTYPE(rv) != SVt_RV) {
        croak("rv is not a reference");
    }

    if (isSVrv_a_MopOV(rv)) {
        return rv;
    }

    MopOV* opaque;

    Newx(opaque, 1, MopOV);
    opaque->id        = new_uuid();
    opaque->slots     = newHV();
    opaque->callbacks = newHV();

    sv_magicext(SvRV(rv), NULL, PERL_MAGIC_ext, &MopOV_vtbl, (char*) opaque, 0);

    return rv;
}

/* *****************************************************
 * Destructor
 * ***************************************************** */

void THX_freeMopOV(pTHX_ MopOV* opaque) {
    assert(opaque != NULL);

    hv_undef(opaque->slots);
    hv_undef(opaque->callbacks);

    Safefree(opaque->id);
    opaque->id        = NULL;
    opaque->slots     = NULL;
    opaque->callbacks = NULL;

    Safefree(opaque);
    opaque = NULL;
}

/* *****************************************************
 * Slot access
 * ***************************************************** */

SV* THX_MopOV_get_at_slot(pTHX_ SV* rv, SV* slot_name) {
    MopOV* opaque  = SVrv_to_MopOV(rv);
    HE* slot_entry = hv_fetch_ent(opaque->slots, slot_name, 0, 0);
    return slot_entry == NULL ? newSV(0) : HeVAL(slot_entry);
}

void THX_MopOV_set_at_slot(pTHX_ SV* rv, SV* slot_name, SV* slot_value) {
    MopOV* opaque = SVrv_to_MopOV(rv);
    SvREFCNT_inc(slot_value);
    (void)hv_store_ent(opaque->slots, slot_name, slot_value, 0);
}

bool THX_MopOV_has_at_slot(pTHX_ SV* rv, SV* slot_name) {
    MopOV* opaque = SVrv_to_MopOV(rv);
    return hv_exists_ent(opaque->slots, slot_name, 0);
}

/* *****************************************************
 * Slot access
 * ***************************************************** */

SV* THX_MopOV_has_events(pTHX_ SV* rv) {
    MopOV* opaque = SVrv_to_MopOV(rv);
    return newSViv(HvKEYS(opaque->callbacks));
}

void THX_MopOV_bind_event(pTHX_ SV* rv, SV* event_name, SV* callback) {

    if (SvTYPE(callback) != SVt_RV || SvTYPE(SvRV(callback)) != SVt_PVCV) {
        croak("callback is not a CODE reference");
    }

    MopOV* opaque;
    AV* events;

    opaque = SVrv_to_MopOV(rv);
    events = fetch_events_by_name(opaque->callbacks, event_name);

    if (events == NULL) {
        events = newAV();
        (void)hv_store_ent(opaque->callbacks, event_name, newRV_noinc((SV*) events), 0);
    }

    av_push(events, SvREFCNT_inc(callback));
}

void THX_MopOV_unbind_event(pTHX_ SV* rv, SV* event_name, SV* callback) {

    if (SvTYPE(callback) != SVt_RV || SvTYPE(SvRV(callback)) != SVt_PVCV) {
        croak("callback is not a CODE reference");
    }

    MopOV* opaque;
    AV* events;

    opaque = SVrv_to_MopOV(rv);
    events = fetch_events_by_name(opaque->callbacks, event_name);

    if (events != NULL) {

        AV* new_events;
        I32 event_array_length, i;

        event_array_length = av_top_index(events);

        if (event_array_length != -1) {
            new_events = newAV();
        }

        for (i = 0; i <= event_array_length; i++) {
            SV* event_cb;

            event_cb = (SV*) *av_fetch(events, i, 0);
            if (SvRV(event_cb) == SvRV(callback)) {
                (void)av_delete(events, i, 0);
            } else {
                av_push(new_events, event_cb);
            }
        }

        if (event_array_length != -1) {
            (void)hv_delete_ent(opaque->callbacks, event_name, G_DISCARD, 0);
            if (av_top_index(new_events) == -1) {
                av_undef(new_events);
            } else {
                (void)hv_store_ent(opaque->callbacks, event_name, newRV_noinc((SV*) new_events), 0);
            }
        }

    }
}

void THX_MopOV_fire_event(pTHX_ SV* rv, SV* event_name, SV** args, I32 args_len) {
    MopOV* opaque;
    AV* events;

    opaque = SVrv_to_MopOV(rv);
    events = fetch_events_by_name(opaque->callbacks, event_name);

    if (events != NULL) {

        I32 event_array_length, i, j;

        event_array_length = av_len(events);

        if (event_array_length != -1) {

            dSP;

            for (i = 0; i <= event_array_length; i++) {
                SV* code;
                code = (SV*) *av_fetch(events, i, 0);

                ENTER;
                SAVETMPS;
                PUSHMARK(SP);
                for (j = 0; j <= args_len; j++) {
                    XPUSHs(args[j]);
                }
                PUTBACK;
                (void)call_sv(code, G_VOID|G_DISCARD);
                SPAGAIN;
                FREETMPS;
                LEAVE;
            }
        }
    }
}

/* *****************************************************
 * Util functions ...
 * ***************************************************** */

bool isSVrv_a_MopOV(SV* rv) {
    assert(rv != NULL);

    if (SvTYPE(rv) != SVt_RV && SvTYPE(SvRV(rv)) != SVt_PVMG) {
        croak("rv is not a magic reference");
    }

    if (SvMAGICAL(SvRV(rv))) {
        MAGIC* mg;
        for (mg = SvMAGIC(SvRV(rv)); mg; mg = mg->mg_moremagic) {
            if ((mg->mg_type == PERL_MAGIC_ext) && (mg->mg_virtual == &MopOV_vtbl)) {
                return mg->mg_ptr != NULL ? TRUE : FALSE;
            }
        }
    }

    return false;
}

MopOV* SVrv_to_MopOV(SV* rv) {
    assert(rv != NULL);

    if (SvTYPE(rv) != SVt_RV && SvTYPE(SvRV(rv)) != SVt_PVMG) {
        croak("rv is not a magic reference");
    }

    if (SvMAGICAL(SvRV(rv))) {
        MAGIC* mg;
        for (mg = SvMAGIC(SvRV(rv)); mg; mg = mg->mg_moremagic) {
            if ((mg->mg_type == PERL_MAGIC_ext) && (mg->mg_virtual == &MopOV_vtbl)) {
                return (MopOV*) mg->mg_ptr;
            }
        }
    }

    croak("rv is not a mop instance");
}

/* *****************************************************
 * Internal Util functions ...
 * ***************************************************** */

// magic destructor ...
static int mg_freeMopOV(pTHX_ SV *sv, MAGIC *mg) {

    // XXX:
    // guard against global destruction
    // in here (not exactly sure how, but
    // Vincent suggested it).
    // - SL

    if (SvREFCNT(sv) == 0) {
        freeMopOV((MopOV*) mg->mg_ptr);
        mg->mg_ptr = NULL;
    }
    return 0;
}

// find/create the events array in the callbacks HV
static AV* fetch_events_by_name (HV* callbacks, SV* event_name) {
    AV* events;

    HE* events_entry = hv_fetch_ent(callbacks, event_name, 0, 0);
    if (events_entry != NULL) {
        events = (AV*) SvRV(HeVAL(events_entry));
        if (SvTYPE(events) != SVt_PVAV) {
            croak("events is not an arrayref");
        }
        return events;
    } else {
        return NULL;
    }
}

// Quick simple (and wrong) UUID mechanism, this will get
// replaced, but sufficient for now,
static I32* new_uuid() {
    I32* uuid;
    int i;

    Newx(uuid, 4, I32);

    if (!PL_srand_called) {
        (void)seedDrand01((Rand_seed_t)Perl_seed(aTHX));
        PL_srand_called = TRUE;
    }

    for (i = 0; i < 4; ++i) {
        uuid[i] = (I32)(Drand01() * (double)(2<<30));
    }

    return uuid;
}