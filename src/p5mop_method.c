#include "p5mop.h"
#include "p5mop_method.h"

static void _MopMmV_wrapper (pTHX_ CV *cv);

/* *****************************************************
 * Constructors
 * ***************************************************** */

SV* THX_newMopMmV(pTHX_ SV* code, U32 flags) {
    SV* method;

    CV* cv = (CV*) newSV(0);
    sv_upgrade((SV*) cv, SVt_PVCV);
    CvISXSUB_on(cv);
    CvXSUB(cv) = _MopMmV_wrapper;
    CvXSUBANY(cv).any_uv = PTR2UV(code);
    CvFILE(cv) = __FILE__;    

    CvANON_off(cv);
    CvMETHOD_on(cv);

    SvREFCNT_inc(code);

    method = newMopOV(newRV_inc((SV*) cv));

    if (flags & MopMmVf_STEAL_STASH) {
        MopMmV_assign_to_stash(method, CvGV(SvRV(code)), CvSTASH(SvRV(code)));
    }

    return method;
}

/* *****************************************************
 * Installers
 * ***************************************************** */

void THX_MopMmV_assign_to_stash(pTHX_ SV* metamethod, GV* gv, HV* stash) {
    CV* cv = (CV*) SvRV(metamethod);

    GvCVGEN(gv) = 0;
    GvCV_set(gv, cv);
    CvGV_set(cv, gv);
    CvSTASH_set(cv, stash);
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
 * Method wrapper to handle events 
 * ***************************************************** 
 * So this is (hopefully) doing a lot more then it 
 * actually needs to. In particular all the messing 
 * around with the stack and storing it in the 
 * temporary results array, etc. I suspect too that 
 * the args fiddling with before/after:EXECUTE are 
 * not efficient either, though creating the SVpv 
 * for the calls to fire_event seemed insignificant
 * when I benchmarked. Either way, need someone with 
 * a lot more knowledge of Perl's calling conventions
 * to take a look at this and sanity check it.
 * ***************************************************** */

static void _MopMmV_wrapper (pTHX_ CV *cv) {
    bool has_events;
    I32  j, count;
    SV** args;
    CV*  body;
    SV*  object  = newRV_noinc((SV*) cv);
    AV*  results = newAV();
    
    dXSARGS;

    has_events = MopOV_has_events(object);
    body       = (CV*) CvXSUBANY(cv).any_uv;

    if (SvTYPE(body) != SVt_PVCV) {
        croak("body is not a code ref");
    }

    if (has_events) {
        Newx(args, items, SV*);
        for (j = 0; j < items; j++) {
            args[j] = ST(j);
        }
        MopOV_fire_event(object, newSVpv("before:EXECUTE", 14), args, items-1);
    }

    {
        ENTER;
        PUSHMARK(SP);
        for (j = 0; j < items; j++) {
            PUSHs(args[j]);
        }
        PUTBACK;
        count = call_sv((SV*) body, GIMME_V);
        SPAGAIN;

        while (count-- > 0) {
            av_push(results, POPs);
        }

        LEAVE;
    }

    for (j = 0; j < av_len(results) + 1; j++) {
        ST(j) = *av_fetch(results, av_len(results) - j, 0);
    }

    if (has_events) {
        MopOV_fire_event(object, newSVpv("after:EXECUTE", 13), args, items-1);   
    }

    XSRETURN(av_len(results) + 1);
}
