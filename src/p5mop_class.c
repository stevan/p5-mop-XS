#include "p5mop.h"
#include "p5mop_class.h"

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopMcV(pTHX_ const char* name, const U32 name_len) {
    return newMopOV(newRV_inc((SV*) gv_stashpvn(name, name_len, GV_ADD)));
}

/* *****************************************************
 * Accessors
 * ***************************************************** */


SV* THX_MopMcV_get_name(pTHX_ SV* metaclass) {
    return newSVpv(HvNAME(SvRV(metaclass)), 0);
}


SV* THX_MopMcV_get_version(pTHX_ SV* metaclass) {
    HV* stash = (HV*) SvRV(metaclass);

    SV** version = hv_fetch(stash, "VERSION", 7, 0);
    if (version != NULL) {
        return GvSV((GV*) *version);
    } else {
        return NULL;
    }
}


SV* THX_MopMcV_get_authority(pTHX_ SV* metaclass) {
    HV* stash = (HV*) SvRV(metaclass);

    SV** authority = hv_fetch(stash, "AUTHORITY", 9, 0);
    if (authority != NULL) {
        return GvSV((GV*) *authority);
    } else {
        return NULL;
    }
}

// superclass

SV* THX_MopMcV_get_superclass(pTHX_ SV* metaclass) {
    HV* stash = (HV*) SvRV(metaclass);

    SV** isa_gv = hv_fetch(stash, "ISA", 3, 0);
    if (isa_gv != NULL) {
        AV* isa_av = GvAV((GV*) *isa_gv);
        if (isa_av != NULL) {
            SV** super = av_fetch(isa_av, 0, 0);
            if (super != NULL) {
                STRLEN name_len;
                char*  name = SvPV(*super, name_len);
                return newMopMcV(name, name_len);
            }
        }
    }
    return NULL;
}

void THX_MopMcV_set_superclass(pTHX_ SV* metaclass, SV* superclass) {
    AV* isa_av;
    GV* isa_gv;
    HV* stash = (HV*) SvRV(metaclass);

    SV** isa_gv_p = hv_fetch(stash, "ISA", 3, 0);

    if (isa_gv_p != NULL) {
        isa_gv = (GV*) *isa_gv_p;
    } else {
        isa_gv = (GV*) newSV(0);
        gv_init_pvn(isa_gv, stash, "ISA", 3, 0);
        (void)hv_store(stash, "ISA", 3, (SV*) isa_gv, 0);
    }

    isa_av = GvAV(isa_gv);
    if (isa_av == NULL) {
        gv_AVadd(isa_gv);
        isa_av = GvAV(isa_gv);
    }

    (void)av_store(isa_av, 0, MopMcV_get_name(superclass));
}

// methods

bool THX_MopMcV_has_method(pTHX_ SV* metaclass, SV* name) {
    HV* stash = (HV*) SvRV(metaclass);

    HE* method_gv_he = hv_fetch_ent(stash, name, 0, 0);
    if (method_gv_he != NULL) {
        GV* method_gv = (GV*) HeVAL(method_gv_he);
        CV* method    = GvCV(method_gv);
        if (method != NULL && GvSTASH(CvGV(method)) == stash) {
            return TRUE;
        }
    }
    
    return FALSE;
}

SV* THX_MopMcV_get_method(pTHX_ SV* metaclass, SV* name) {
    HV* stash = (HV*) SvRV(metaclass);

    HE* method_gv_he = hv_fetch_ent(stash, name, 0, 0);
    if (method_gv_he != NULL) {
        GV* method_gv = (GV*) HeVAL(method_gv_he);
        CV* method    = GvCV(method_gv);
        if (method != NULL && GvSTASH(CvGV(method)) == stash) {
            return newRV_inc((SV*) method);  
        }
    }
    
    return NULL;
}

SV* THX_MopMcV_upgrade_method(pTHX_ SV* metaclass, SV* code) {
    return newMopMmV(code, boolSV(TRUE));
}

void THX_MopMcV_add_method(pTHX_ SV* metaclass, SV* name, SV* code) {
    GV* method_gv;
    HV* stash  = (HV*) SvRV(metaclass);
    SV* method = newMopMmV(code, boolSV(false));

    HE* method_gv_he = hv_fetch_ent(stash, name, 0, 0);
    if (method_gv_he != NULL) {
        method_gv = (GV*) HeVAL(method_gv_he);
    } else {
        method_gv = (GV*) newSV(0);
        gv_init_sv(method_gv, stash, name, 0);
        (void)hv_store_ent(stash, name, (SV*) method_gv, 0);
    }

    MopMmV_assign_to_stash(method, method_gv, stash);
}

/* *****************************************************
 * Methods
 * ***************************************************** */

SV* THX_MopMcV_construct_instance(pTHX_ SV* metaclass, SV* repr) {
    return sv_bless(repr, (HV*) SvRV(metaclass));
}

/* *****************************************************
 * Internal Util functions ...
 * ***************************************************** */


