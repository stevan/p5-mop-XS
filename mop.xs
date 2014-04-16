#include "EXTERN.h"
#include "perl.h"
#define NO_XSLOCKS
#include "XSUB.h"

#include "p5mop.h"
#include "p5mop.c"

#include "p5mop_method.h"
#include "p5mop_method.c"

#include "p5mop_attribute.h"
#include "p5mop_attribute.c"

#include "p5mop_class.h"
#include "p5mop_class.c"

MODULE = mop  PACKAGE = mop::internals

SV* 
newMopMcV(name)
    SV* name; 

void
newMopMmV(code)
    SV* code; 
    PPCODE:
        (void)newMopMmV(code);
        XSRETURN(1);

SV* 
newMopMaV(name)
    SV* name; 

void
newMopOV(rv)
    SV* rv;
    PPCODE:
        (void)newMopOV(rv);
        XSRETURN(1);

SV*
newMopOVsv();

SV*
newMopOVhv();

SV*
newMopOVav();

MODULE = mop  PACKAGE = mop::internals::MopOV

void
get_at_slot(rv, slot_name)
    SV* rv;
    SV* slot_name;
    PPCODE:
        EXTEND(SP, 1);
        PUSHs(MopOV_get_at_slot(rv, slot_name));

void
set_at_slot(rv, slot_name, slot_value)
    SV* rv;
    SV* slot_name;
    SV* slot_value;
    CODE:
        MopOV_set_at_slot(rv, slot_name, slot_value);
        XSRETURN(2);

SV*
has_at_slot(rv, slot_name)
    SV* rv;
    SV* slot_name;
    CODE:
        RETVAL = boolSV(MopOV_has_at_slot(rv, slot_name));
    OUTPUT:
        RETVAL

SV*
has_events(object)
    SV* object;
    CODE:
        RETVAL = MopOV_has_events(object);
    OUTPUT:
        RETVAL

void
bind_event(object, event_name, callback)
    SV *object;
    SV *event_name;
    SV *callback;
    CODE:
        MopOV_bind_event(object, event_name, callback);
        XSRETURN(1);

void
unbind_event(object, event_name, callback)
    SV *object;
    SV *event_name;
    SV *callback;
    CODE:
        MopOV_unbind_event(object, event_name, callback);
        XSRETURN(1);

void
fire_event(object, event_name, ...)
    SV* object;
    SV* event_name;
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

MODULE = mop  PACKAGE = mop::internals::MopMcV

SV*
name(metaclass)
    SV* metaclass;
    CODE:
        RETVAL = MopMcV_get_name(metaclass);
    OUTPUT:
        RETVAL

void
version(metaclass)
    SV* metaclass;
    PPCODE:
        EXTEND(SP, 1);
        PUSHs(MopMcV_get_version(metaclass));

void
authority(metaclass)
    SV* metaclass;
    PPCODE:
        EXTEND(SP, 1);
        PUSHs(MopMcV_get_authority(metaclass));

SV*
superclass(metaclass)
    SV* metaclass;
    CODE:
        RETVAL = MopMcV_get_superclass(metaclass);
    OUTPUT:
        RETVAL

void
construct_instance(metaclass, repr)
    SV* metaclass;
    SV* repr;
    PPCODE:
        EXTEND(SP, 1);
        PUSHs(MopMcV_construct_instance(metaclass, repr));

MODULE = mop  PACKAGE = mop::internals::MopMmV

SV*
name(metamethod)
    SV* metamethod;
    CODE:
        RETVAL = MopMmV_get_name(metamethod);
    OUTPUT:
        RETVAL

void
associated_class(metamethod)
    SV* metamethod;
    PPCODE:
        EXTEND(SP, 1);
        PUSHs(MopMmV_get_associated_class(metamethod));

MODULE = mop  PACKAGE = mop::internals::MopMaV

SV*
name(meta_attr)
    SV* meta_attr;
    CODE:
        RETVAL = MopMaV_get_name(meta_attr);
    OUTPUT:
        RETVAL

