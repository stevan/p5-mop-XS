#include "p5mop.h"
#include "p5mop_class.h"

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopMCV(pTHX_ SV* name) {
    return newMopOV(newRV_noinc((SV*) gv_stashsv(name, GV_ADD)));
}

/* *****************************************************
 * Accessors
 * ***************************************************** */


SV* THX_MopMCV_get_name(pTHX_ SV* metaclass) {
	return newSVpv(HvNAME(SvRV(metaclass)), 0);
}


SV* THX_MopMCV_get_version(pTHX_ SV* metaclass) {
	HV* stash = (HV*) SvRV(metaclass);

	SV** version = hv_fetch(stash, "VERSION", 7, 0);
	if (version != NULL) {
		return GvSV((GV*) *version);
	} else {
		return &PL_sv_undef;
	}
}


SV* THX_MopMCV_get_authority(pTHX_ SV* metaclass) {
	HV* stash = (HV*) SvRV(metaclass);

	SV** authority = hv_fetch(stash, "AUTHORITY", 9, 0);
	if (authority != NULL) {
		return GvSV((GV*) *authority);
	} else {
		return &PL_sv_undef;
	}
}

/* *****************************************************
 * Internal Util functions ...
 * ***************************************************** */


