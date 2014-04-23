#include "p5mop.h"
#include "p5mop_method.h"

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopMmV(pTHX_ SV* code) {
    return newMopOV(code);
}

/* *****************************************************
 * Accessors
 * ***************************************************** */

SV* THX_MopMmV_get_name(pTHX_ SV* metamethod) {
    return newSVpv(GvNAME(CvGV((CV*) SvRV(metamethod))), 0);
}

SV* THX_MopMmV_get_associated_class(pTHX_ SV* metamethod) {
    return newRV_noinc((SV*) CvSTASH(SvRV(metamethod)));
}