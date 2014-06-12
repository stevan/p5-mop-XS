#!/usr/bin/env perl

use v5.18;
use warnings;

use Benchmark qw[ cmpthese ];

use mop;

sub no_mop       { 2 + 2 == 4 }
sub mop          { 2 + 2 == 4 }
sub mop_w_events { 2 + 2 == 4 }

{
    my $m = mop::internals::newMopMmV(\&mop);
}

{
    my $x = 0;
    my $m = mop::internals::newMopMmV(\&mop_w_events);
    mop::internals::MopOV::bind_event($m, 'before:EXECUTE', sub { $x++ });
    mop::internals::MopOV::bind_event($m, 'after:EXECUTE', sub { $x-- });
}

cmpthese(5_000_000, {
    'no mop'        => \&no_mop,
    'mop'           => \&mop,
    'mop w/ events' => \&mop_w_events,
});

