#include "EXTERN.h"
#include "perl.h"
#define NO_XSLOCKS
#include "XSUB.h"

#include "src/p5mop.h"

MODULE = mop  PACKAGE = mop::internals

void
newMopOV(rv)
    SV* rv;

SV*
newMopOVsv();

SV*
newMopOVhv();

SV*
newMopOVav();

MODULE = mop  PACKAGE = mop::internals::slots

void
get(rv, slot_name)
    SV* rv;
    SV* slot_name;
    PPCODE:
        EXTEND(SP, 1);
        PUSHs(MopOV_get_at_slot(rv, slot_name));

void
set(rv, slot_name, slot_value)
    SV* rv;
    SV* slot_name;
    SV* slot_value;
    CODE:
        MopOV_set_at_slot(rv, slot_name, slot_value);
        XSRETURN(2);

SV*
has(rv, slot_name)
    SV* rv;
    SV* slot_name;
    CODE:
        RETVAL = boolSV(MopOV_has_at_slot(rv, slot_name));
    OUTPUT:
        RETVAL

MODULE = mop  PACKAGE = mop::internals::events

SV*
has_events(object)
    SV* object;
    CODE:
        RETVAL = MopOV_has_events(object);
    OUTPUT:
        RETVAL

void
bind(object, event_name, callback)
    SV *object
    SV *event_name
    SV *callback
    CODE:
        MopOV_bind_event(object, event_name, callback);
        XSRETURN(1);

void
unbind(object, event_name, callback)
    SV *object
    SV *event_name
    SV *callback
    CODE:
        MopOV_unbind_event(object, event_name, callback);
        XSRETURN(1);

void
fire(object, event_name, ...)
    SV* object
    SV* event_name
    PREINIT:
        I32 j;
        SV** args;
    CODE:
        Newx(args, items, SV*);
        for (j = 0; j <= items; j++) {
            args[j] = ST(j);
        }
        MopOV_fire_event(object, event_name, args, items);
        Safefree(args);
        XSRETURN(1);




