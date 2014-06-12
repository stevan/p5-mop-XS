#!/usr/bin/env perl

use v5.18;
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

    ok(!mop::internals::MopOV::has_at_slot($o, "foo"), '... no value at slot "foo"');

    mop::internals::MopOV::set_at_slot($o, "foo", 10);
    ok(mop::internals::MopOV::has_at_slot($o, "foo"), '... have a value at slot "foo" now');

    is(mop::internals::MopOV::get_at_slot($o, "foo"), 10, '... got the correct value for slot "foo"');

    ok(!mop::internals::MopOV::has_at_slot($o, "bar"), '... no value at slot "bar"');
    is(mop::internals::MopOV::get_at_slot($o, "bar"), undef, '... got the correct value for slot "bar"');
    ok(!mop::internals::MopOV::has_at_slot($o, "bar"), '... still no value at slot "bar"');

    ok(!mop::internals::MopOV::has_events($o), '... no events yet');

    mop::internals::MopOV::set_at_slot($o, "test", 0);
    ok(mop::internals::MopOV::has_at_slot($o, "test"), '... have a value at slot "test" now');

    my $test_event = sub {
        my $o = shift;
        mop::internals::MopOV::set_at_slot($o, "test", mop::internals::MopOV::get_at_slot($o, "test") + 1);
    };

    mop::internals::MopOV::bind_event($o, 'test', $test_event);

    ok(mop::internals::MopOV::has_events($o), '... have events now');
    is(mop::internals::MopOV::get_at_slot($o, "test"), 0, '... test event has not been fired');

    mop::internals::MopOV::fire_event($o, 'test' => ($o));

    is(mop::internals::MopOV::get_at_slot($o, "test"), 1, '... test event has been fired');

    mop::internals::MopOV::unbind_event($o, 'test', $test_event);

    ok(!mop::internals::MopOV::has_events($o), '... no events anymore');

    mop::internals::MopOV::fire_event($o, 'test' => ($o));
    is(mop::internals::MopOV::get_at_slot($o, "test"), 1, '... test event was not fired again');

    ok(mop::internals::MopOV::has_at_slot($o, "test"), '... no longer have a value at slot "test"');
    mop::internals::MopOV::clear_at_slot($o, "test");
    ok(!mop::internals::MopOV::has_at_slot($o, "test"), '... no longer have a value at slot "test"');
}

{
    my $o = mop::internals::newMopOVsv();

    ok(!mop::internals::MopOV::has_events($o), '... no events yet');

    my @tests;
    my @events;
    foreach my $i (0 .. 10) {
        $tests[$i]  = 0;
        $events[$i] = sub { $tests[$i]++ };

        mop::internals::MopOV::bind_event($o, 'test', $events[$i]);
    }

    ok(mop::internals::MopOV::has_events($o), '... have events now');
    is($tests[$_], 0, '... test ('.$_.') event has not been fired')
        foreach 0 .. 10;

    mop::internals::MopOV::fire_event($o, 'test');
    mop::internals::MopOV::fire_event($o, 'test');
    mop::internals::MopOV::fire_event($o, 'test');

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

