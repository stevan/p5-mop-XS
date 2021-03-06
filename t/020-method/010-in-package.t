#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

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