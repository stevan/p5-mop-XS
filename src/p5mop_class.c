#include "p5mop.h"
#include "p5mop_class.h"

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopMcV(pTHX_ SV* name) {
    return newMopOV(newRV_inc((SV*) gv_stashsv(name, GV_ADD)));
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
                return newMopMcV(*super);
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

// attributes

bool THX_MopMcV_has_attribute(pTHX_ SV* metaclass, SV* name) {
    SV* attributes = MopOV_get_at_slot(metaclass, CLASS_ATTRIBUTE_SLOT);
    if (attributes == NULL) {
        attributes = newRV_noinc((SV*) newHV());                    
        MopOV_set_at_slot(metaclass, CLASS_ATTRIBUTE_SLOT, attributes); 
        // NOTE:
        // I know I am not going to 
        // have the value since I 
        // only just now created the
        // HV to store it.
        return FALSE;
    }

    if (SvTYPE(attributes) != SVt_RV && SvTYPE(SvRV(attributes)) != SVt_PVHV) {
        croak("attributes is not a HASH ref, this is wrong");
    }

    return hv_exists_ent((HV*) SvRV(attributes), name, 0);
}

SV* THX_MopMcV_get_attribute(pTHX_ SV* metaclass, SV* name) {
    HE* attribute;

    SV* attributes = MopOV_get_at_slot(metaclass, CLASS_ATTRIBUTE_SLOT);
    if (attributes == NULL) {
        attributes = newRV_noinc((SV*) newHV());                    
        MopOV_set_at_slot(metaclass, CLASS_ATTRIBUTE_SLOT, attributes); 
        // NOTE:
        // I know I am not going to 
        // have the value since I 
        // only just now created the
        // HV to store it.
        return NULL;
    }

    if (SvTYPE(attributes) != SVt_RV && SvTYPE(SvRV(attributes)) != SVt_PVHV) {
        croak("attributes is not a HASH ref, this is wrong");
    }

    attribute = hv_fetch_ent((HV*) SvRV(attributes), name, 0, 0);
    return attribute == NULL ? NULL : HeVAL(attribute);
}

void THX_MopMcV_add_attribute(pTHX_ SV* metaclass, SV* attribute) {
    SV* attr_name;

    SV* attributes = MopOV_get_at_slot(metaclass, CLASS_ATTRIBUTE_SLOT);
    if (attributes == NULL) {
        attributes = newRV_noinc((SV*) newHV());                    
        MopOV_set_at_slot(metaclass, CLASS_ATTRIBUTE_SLOT, attributes); 
    }

    if (SvTYPE(attributes) != SVt_RV && SvTYPE(SvRV(attributes)) != SVt_PVHV) {
        croak("attributes is not a HASH ref, this is wrong");
    }

    attr_name = MopMaV_get_name(attribute);
    if (attr_name == NULL) {
        croak("The attribute has no name, this is wrong!");
    }

    if (NULL == hv_store_ent((HV*) SvRV(attributes), attr_name, attribute, 0)) {
        croak("The attribute failed to store, this is wrong!");
    } 

    MopMaV_set_associated_class(attribute, metaclass);
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
    return newMopMmV(code, MopMmVf_STEAL_STASH);
}

void THX_MopMcV_add_method(pTHX_ SV* metaclass, SV* name, SV* code) {
    GV* method_gv;
    HV* stash  = (HV*) SvRV(metaclass);
    SV* method = newMopMmV(code, 0);

    HE* method_gv_he = hv_fetch_ent(stash, name, 0, 0);
    if (method_gv_he != NULL) {
        GV* method_gv = (GV*) HeVAL(method_gv_he);
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
    // TODO:
    // This should handle all the attributes
    // and constructing things properly, which
    // should also include running all BUILD
    // methods.
    // - SL
    return sv_bless(repr, (HV*) SvRV(metaclass));
}

/* *****************************************************
 * Internal Util functions ...
 * ***************************************************** */


