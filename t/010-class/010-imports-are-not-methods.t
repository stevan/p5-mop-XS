#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

package My::ImportTest {
    use Data::Dumper qw[ Dumper ];
    sub test { __PACKAGE__ . '::test' }
}

{
    my $mcv = mop::internals::newMopMcV("My::ImportTest");

    ok(mop::internals::MopMcV::has_method($mcv, 'test'), '... we have the &test method');
    ok(!mop::internals::MopMcV::has_method($mcv, 'Dumper'), '... we do not have the imported &Dumper function');

    {
        my $test = mop::internals::MopMcV::get_method($mcv, 'test');
        is(ref($test), 'CODE', '... got a code ref');

        is($test->(), 'My::ImportTest::test', '... got the right value');
    }

    is(mop::internals::MopMcV::get_method($mcv, 'Dumper'), undef, '... nothing back from getting the &Dumper function');
}

done_testing;