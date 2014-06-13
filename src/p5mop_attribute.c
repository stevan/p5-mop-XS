#include "p5mop.h"
#include "p5mop_attribute.h"

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopMaV(pTHX_ SV* name) {
    SV* meta_attr = newMopOVsv();
    SvREFCNT_inc(name);
    MopOV_set_at_slot(meta_attr, ATTR_NAME_SLOT, name);
    return meta_attr;
}

/* *****************************************************
 * Accessors
 * ***************************************************** */

SV* THX_MopMaV_get_name(pTHX_ SV* meta_attr) {
    return MopOV_get_at_slot(meta_attr, ATTR_NAME_SLOT);
}

SV* THX_MopMaV_get_key_name(pTHX_ SV* meta_attr) {
    // FIXME - This is soooooo not UTF-8 safe!!
    const char* name_str;
    STRLEN      name_len;
    SV*         name;
    name = MopMaV_get_name(meta_attr);
    if (name != NULL) {
        name_str = SvPV(name, name_len);
        return newSVpv(name_str+=2, name_len-2);
    }
    return NULL;
}

// metaclasses

SV* THX_MopMaV_get_associated_class(pTHX_ SV* meta_attr) {
    return MopOV_get_at_slot(meta_attr, ATTR_ASSOC_META_SLOT);
}

void THX_MopMaV_set_associated_class(pTHX_ SV* meta_attr, SV* metaclass) {
    // we actually want this to be 
    // weak, so we don't inc the 
    // ref count here. - SL
    MopOV_set_at_slot(meta_attr, ATTR_ASSOC_META_SLOT, metaclass);
}

// default values

bool THX_MopMaV_has_default_generator(pTHX_ SV* meta_attr) {
    return MopOV_has_at_slot(meta_attr, ATTR_DEFAULT_SLOT);
}

SV* THX_MopMaV_get_default_generator(pTHX_ SV* meta_attr) {
    return MopOV_get_at_slot(meta_attr, ATTR_DEFAULT_SLOT);
}

void THX_MopMaV_set_default_generator(pTHX_ SV* meta_attr, SV* generator) {
    if (SvTYPE(generator) != SVt_RV || SvTYPE(SvRV(generator)) != SVt_PVCV) {
        croak("generator is not a CODE reference");
    }
    SvREFCNT_inc(generator);
    MopOV_set_at_slot(meta_attr, ATTR_DEFAULT_SLOT, generator);
}

void THX_MopMaV_clear_default_generator(pTHX_ SV* meta_attr) {
    MopOV_clear_at_slot(meta_attr, ATTR_DEFAULT_SLOT);
}

/* *****************************************************
 * Methods
 * ***************************************************** */

/* *****************************************************
 * Internal Util functions ...
 * ***************************************************** */


