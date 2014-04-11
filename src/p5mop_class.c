#include "p5mop.h"
#include "p5mop_class.h"

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopMCV(pTHX_ SV* name, HV* parent) {
	GV* gv = (GV*) newSV(0);

	gv_init_sv(gv, parent, name, GV_ADDMULTI);

    return newMopOV(newRV_noinc((SV*) gv));
}

/* *****************************************************
 * Accessors
 * ***************************************************** */


SV* THX_MopMCV_get_name(pTHX_ SV* metaclass) {
	GV* gv = (GV*) SvRV(metaclass);
	return newSVpv(GvNAME(gv), GvNAMELEN(gv));
}

/*
SV* THX_MopMCV_get_version(pTHX_ SV* metaclass) {

}

SV* THX_MopMCV_get_authority(pTHX_ SV* metaclass) {

}
*/

/* *****************************************************
 * Internal Util functions ...
 * ***************************************************** */


