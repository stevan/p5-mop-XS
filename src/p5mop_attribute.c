#include "p5mop.h"
#include "p5mop_attribute.h"

#define ATTR_NAME_SLOT newSVpv("$!name", 6)

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

/* *****************************************************
 * Methods
 * ***************************************************** */

/* *****************************************************
 * Internal Util functions ...
 * ***************************************************** */


