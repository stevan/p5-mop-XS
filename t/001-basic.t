#!/usr/bin/env perl

use v5.19;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

{
    my $o = \(my $x);
    mop::internals::newMopOV($o);

    ok($o, '... need a test in here');

    ok(!mop::internals::slots::has($o, "foo"), '... no value at slot "foo"');

    mop::internals::slots::set($o, "foo", 10);
    ok(mop::internals::slots::has($o, "foo"), '... have a value at slot "foo" now');

    is(mop::internals::slots::get($o, "foo"), 10, '... got the correct value for slot "foo"');

    ok(!mop::internals::slots::has($o, "bar"), '... no value at slot "bar"');
    is(mop::internals::slots::get($o, "bar"), undef, '... got the correct value for slot "bar"');
    ok(!mop::internals::slots::has($o, "bar"), '... still no value at slot "bar"');

    ok(!mop::internals::events::has_events($o), '... no events yet');

    mop::internals::slots::set($o, "test", 0);
    ok(mop::internals::slots::has($o, "test"), '... have a value at slot "test" now');

    my $test_event = sub {
        my $o = shift;
        mop::internals::slots::set($o, "test", mop::internals::slots::get($o, "test") + 1);
    };

    mop::internals::events::bind($o, 'test', $test_event);

    ok(mop::internals::events::has_events($o), '... have events now');
    is(mop::internals::slots::get($o, "test"), 0, '... test event has not been fired');

    mop::internals::events::fire($o, 'test');

    is(mop::internals::slots::get($o, "test"), 1, '... test event has been fired');

    mop::internals::events::unbind($o, 'test', $test_event);

    ok(!mop::internals::events::has_events($o), '... no events anymore');

    mop::internals::events::fire($o, 'test');
    is(mop::internals::slots::get($o, "test"), 1, '... test event was not fired again');
}

{
    my $o = mop::internals::newMopOVsv();

    ok(!mop::internals::events::has_events($o), '... no events yet');

    my @tests;
    my @events;
    foreach my $i (0 .. 10) {
        $tests[$i]  = 0;
        $events[$i] = sub { $tests[$i]++ };

        mop::internals::events::bind($o, 'test', $events[$i]);
    }

    ok(mop::internals::events::has_events($o), '... have events now');
    is($tests[$_], 0, '... test ('.$_.') event has not been fired')
        foreach 0 .. 10;

    mop::internals::events::fire($o, 'test');
    mop::internals::events::fire($o, 'test');
    mop::internals::events::fire($o, 'test');

    is($tests[$_], 3, '... test ('.$_.') event has not been fired')
        foreach 0 .. 10;
}

{
    my $s = mop::internals::newMopOVsv();
    is(ref($s), 'SCALAR', '... got a SCALAR ref instance');

    my $h = mop::internals::newMopOVhv();
    is(ref($h), 'HASH', '... got a HASH ref instance');

    my $a = mop::internals::newMopOVav();
    is(ref($a), 'ARRAY', '... got an ARRAY ref instance');
}


done_testing;

