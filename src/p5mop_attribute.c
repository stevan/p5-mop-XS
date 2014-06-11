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
    // FIXME - so not UTF-8 safe!
    const char* name_str;
    STRLEN name_len;
    name_str = SvPV(MopMaV_get_name(meta_attr), name_len);
    return newSVpv(name_str+=2, name_len-2);
}

SV* THX_MopMaV_get_associated_class(pTHX_ SV* meta_attr) {
    return MopOV_get_at_slot(meta_attr, ASSOC_META_NAME_SLOT);
}

void THX_MopMaV_set_associated_class(pTHX_ SV* meta_attr, SV* metaclass) {
    // we actually want this to be 
    // weak, so we don't inc the 
    // ref count here. - SL
    MopOV_set_at_slot(meta_attr, ASSOC_META_NAME_SLOT, metaclass);
}

/* *****************************************************
 * Methods
 * ***************************************************** */

/* *****************************************************
 * Internal Util functions ...
 * ***************************************************** */


