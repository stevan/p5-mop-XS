#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

sub foo { 
    warn "INSIDE:", Dumper \@_;
    $_[0] + 10 }

{
    my $m = mop::internals::newMopMmV(\&foo);

    my ($before, $after) = (0, 0);
    mop::internals::MopOV::bind_event($m, 'before:EXECUTE', sub { warn "BEFORE", Dumper \@_; $before++ });
    mop::internals::MopOV::bind_event($m, 'after:EXECUTE', sub { warn "AFTER", Dumper \@_; $after++ });

    is($m->(5), 15, '... got the right value calling CODE->() w/ events');
    is($before, 1, '... our before:EXECUTE event fired');
    is($after, 1, '... our after:EXECUTE event fired');

    is(foo(20), 30, '... got the right value calling foo() w/ events');
    is($before, 2, '... our before:EXECUTE event fired');
    is($after, 2, '... our after:EXECUTE event fired');
}

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

package Foo::Bar 0.01 {
    sub baz { __PACKAGE__ . '::baz' }
}

{
    my $m = mop::internals::newMopMmV(\&Foo::Bar::baz);

    my ($before, $after) = (0, 0);
    mop::internals::MopOV::bind_event($m, 'before:EXECUTE', sub { $before++ });
    mop::internals::MopOV::bind_event($m, 'after:EXECUTE', sub { $after++ });

    is($m->(), 'Foo::Bar::baz', '... got the right value');
    is($before, 1, '... our before:EXECUTE event fired');
    is($after, 1, '... our after:EXECUTE event fired');

    is(Foo::Bar::baz(), 'Foo::Bar::baz', '... got the right value');
    is($before, 2, '... our before:EXECUTE event fired');
    is($after, 2, '... our after:EXECUTE event fired');

    is(mop::internals::MopMmV::name($m), 'baz', '... got the right name');

    my $c = mop::internals::MopMmV::associated_class($m);

    is(mop::internals::MopMcV::name($c), 'Foo::Bar', '... got the associated class name correctly');
    is(mop::internals::MopMcV::version($c), '0.01', '... got the associated class version correctly');

    {
        my $mcv = mop::internals::newMopMcV("Foo::Bar");

        mop::internals::MopOV::set_at_slot($mcv, "$!test", 10);
        is(mop::internals::MopOV::get_at_slot($mcv, "$!test"), 10, '... got the value stored in magic');
    }

    is(mop::internals::MopOV::get_at_slot($c, "$!test"), 10, '... got the value stored in magic (from diff variable)');

}

done_testing;