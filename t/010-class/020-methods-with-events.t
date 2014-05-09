#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

package Foo::Bar::Baz 0.01 {
    sub test { __PACKAGE__ . '::test' }
}

{
    my $mcv = mop::internals::newMopMcV("Foo::Bar::Baz");

    ok(mop::internals::MopMcV::has_method($mcv, 'test'), '... we have the &test method');

    my $test = mop::internals::MopMcV::upgrade_method(
        $mcv, 
        mop::internals::MopMcV::get_method($mcv, 'test')
    );

    is($test->(), 'Foo::Bar::Baz::test', '... got the right value calling as CODE->()');

    my ($before, $after) = (0, 0);
    mop::internals::MopOV::bind_event($test, 'before:EXECUTE', sub { $before++ });
    mop::internals::MopOV::bind_event($test, 'after:EXECUTE', sub { $after++ });

    is($test->(), 'Foo::Bar::Baz::test', '... got the right value calling as CODE->() w/ events');
    is($before, 1, '... our before:EXECUTE event fired');
    is($after, 1, '... our after:EXECUTE event fired');

    # call method ...
    my $baz = mop::internals::MopMcV::construct_instance($mcv, \(my $x));

    isa_ok($baz, 'Foo::Bar::Baz');
    can_ok($baz, 'test');

    is($baz->test, 'Foo::Bar::Baz::test', '... got the right value calling as $obj->test w/ events');
    is($before, 2, '... our before:EXECUTE event fired');
    is($after, 2, '... our after:EXECUTE event fired');

    is($test->(), 'Foo::Bar::Baz::test', '... got the right value calling as CODE->() w/ events');
    is($before, 3, '... our before:EXECUTE event fired');
    is($after, 3, '... our after:EXECUTE event fired');

    is($baz->test, 'Foo::Bar::Baz::test', '... got the right value calling as $obj->test w/ events');
    is($before, 4, '... our before:EXECUTE event fired');
    is($after, 4, '... our after:EXECUTE event fired');
}


done_testing;