#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};


# works on as yet to be created packages ...
{
    my $mcv = mop::internals::newMopMcV("Foo::Baz");

    is(mop::internals::MopMcV::name($mcv), 'Foo::Baz', '... got the right name');
    is(mop::internals::MopMcV::version($mcv), undef, '... got the right version');
    is(mop::internals::MopMcV::authority($mcv), undef, '... got the right authority');

    ok(!mop::internals::MopMcV::has_method($mcv, 'test'), '... no methods yet');

    mop::internals::MopMcV::add_method($mcv, 'test', sub {
        'Foo::Baz::test'
    });

    ok(mop::internals::MopMcV::has_method($mcv, 'test'), '... got method now');

    my $baz = mop::internals::MopMcV::construct_instance($mcv, \(my $x));

    isa_ok($baz, 'Foo::Baz');
    can_ok($baz, 'test');

    is($baz->test, 'Foo::Baz::test', '... got the right value');
}




done_testing;