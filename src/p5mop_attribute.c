#include "p5mop.h"
#include "p5mop_attribute.h"

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopMaV(pTHX_ const char* name, STRLEN name_len) {
    SV* meta_attr = newMopOVsv();
    MopOV_set_at_slot(meta_attr, "$!name", 6, newSVpv(name, name_len));
    return meta_attr;
}

/* *****************************************************
 * Accessors
 * ***************************************************** */


SV* THX_MopMaV_get_name(pTHX_ SV* meta_attr) {
    return MopOV_get_at_slot(meta_attr, "$!name", 6);
}

SV* THX_MopMaV_get_associated_class(pTHX_ SV* meta_attr) {
    return MopOV_get_at_slot(meta_attr, "$!associated_meta", 17);
}

void THX_MopMaV_set_associated_class(pTHX_ SV* meta_attr, SV* metaclass) {
    // we actually want this to be 
    // weak, so we don't inc the 
    // ref count here. - SL
    MopOV_set_at_slot(meta_attr, "$!associated_meta", 17, metaclass);
}

/* *****************************************************
 * Methods
 * ***************************************************** */

/* *****************************************************
 * Internal Util functions ...
 * ***************************************************** */


