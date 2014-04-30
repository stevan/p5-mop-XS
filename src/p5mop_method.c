#include "p5mop.h"
#include "p5mop_method.h"

static void _MopMmV_wrapper (pTHX_ CV *cv);

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopMmV(pTHX_ SV* code) {
    GV* gv;
    CV* cv;
    HV* stash;

    cv = (CV*) newSV(0);
    sv_upgrade((SV*) cv, SVt_PVCV);
    CvISXSUB_on(cv);
    CvXSUB(cv) = _MopMmV_wrapper;
    CvXSUBANY(cv).any_uv = PTR2UV(code);
    CvFILE(cv) = __FILE__;

    gv    = CvGV((CV*) SvRV(code));
    stash = CvSTASH((CV*) SvRV(code));

    CvANON_off(cv);
    CvMETHOD_on(cv);
    GvCVGEN(gv) = 0;
    GvCV_set(gv, cv);
    CvGV_set(cv, gv);
    CvSTASH_set(cv, stash);

    SvREFCNT_inc(code);

    return newMopOV(newRV_inc((SV*) cv));
}

/* *****************************************************
 * Accessors
 * ***************************************************** */

SV* THX_MopMmV_get_name(pTHX_ SV* metamethod) {
    return newSVpv(GvNAME(CvGV((CV*) SvRV(metamethod))), 0);
}

SV* THX_MopMmV_get_body(pTHX_ SV* metamethod) {
    return newRV_noinc((SV*) CvXSUBANY((CV*) SvRV(metamethod)).any_uv);
}

SV* THX_MopMmV_get_associated_class(pTHX_ SV* metamethod) {
    return newRV_noinc((SV*) CvSTASH(SvRV(metamethod)));
}

/* *****************************************************
 * Accessors
 * ***************************************************** */

static void _MopMmV_wrapper (pTHX_ CV *cv) {
    bool has_events;
    I32  j, count;
    SV** args;
    CV*  body;
    SV*  object = newRV_noinc((SV*) cv);
    
    dXSARGS;

    has_events = MopOV_has_events(object);
    body       = (CV*) CvXSUBANY(cv).any_uv;

    Newx(args, items, SV*);
    for (j = 0; j <= items; j++) {
        args[j] = ST(j);
    }

    if (has_events) {
        MopOV_fire_event(object, newSVpv("before:EXECUTE", 14), args, items);
    }

    {
        dSP;
        PUSHMARK(SP);
        for (j = 0; j <= items; j++) {
            XPUSHs(args[j]);
        }
        PUTBACK;
        count = call_sv((SV*) body, GIMME_V);

        SPAGAIN;
        PUTBACK;
    }

    if (has_events) {
        MopOV_fire_event(object, newSVpv("after:EXECUTE", 13), args, items);   
    }
}