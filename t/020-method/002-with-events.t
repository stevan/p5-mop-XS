#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

sub foo { $_[0] + 10 }

{
    my $m = mop::internals::newMopMmV(\&foo);

    my ($before, $after) = (0, 0);
    mop::internals::MopOV::bind_event($m, 'before:EXECUTE', sub { $before++ });
    mop::internals::MopOV::bind_event($m, 'after:EXECUTE', sub { $after++ });

    is($m->(5), 15, '... got the right value calling CODE->() w/ events');
    is($before, 1, '... our before:EXECUTE event fired');
    is($after, 1, '... our after:EXECUTE event fired');

    is(foo(20), 30, '... got the right value calling foo() w/ events');
    is($before, 2, '... our before:EXECUTE event fired');
    is($after, 2, '... our after:EXECUTE event fired');
}

# test multiple return args work 

sub bar { ($_[0] + 10, $_[0] - 5) }

{
    my $m = mop::internals::newMopMmV(\&bar);

    my ($before, $after) = (0, 0);
    mop::internals::MopOV::bind_event($m, 'before:EXECUTE', sub { $before++ });
    mop::internals::MopOV::bind_event($m, 'after:EXECUTE', sub { $after++ });

    #warn Dumper [ $m->(5) ];

    is_deeply([ $m->(5) ], [ 15, 0 ], '... got the right value');
    is($before, 1, '... our before:EXECUTE event fired');
    is($after, 1, '... our after:EXECUTE event fired');

    is_deeply([ bar(20) ], [ 30, 15 ], '... got the right value');
    is($before, 2, '... our before:EXECUTE event fired');
    is($after, 2, '... our after:EXECUTE event fired');
}

done_testing;