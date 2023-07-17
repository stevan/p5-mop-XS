#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek qw[ Dump SvREFCNT ];

BEGIN {
    use_ok('mop')
};

package Foo::Bar 0.01 {}

{
    my $a = mop::internals::newMopMaV('$!test');

    is(mop::internals::MopMaV::name($a), '$!test', '... got the right name');
    is(mop::internals::MopMaV::key_name($a), 'test', '... got the right key name');
    is(mop::internals::MopMaV::name($a), '$!test', '... still got the right name');

    is(SvREFCNT($a), 1, '... the attribute only has one reference count');

    {
        my $mcv = mop::internals::newMopMcV("Foo::Bar");

        ok(!mop::internals::MopOV::has_at_slot($mcv, '$!attributes'), '... [OV] no $!attributes slot yet');
        ok(!mop::internals::MopMcV::has_attribute($mcv, '$!test'), '... we do not have the attribute yet');
        ok(mop::internals::MopOV::has_at_slot($mcv, '$!attributes'), '... [OV] have $!attributes slot now');

        is(
            mop::internals::MopMcV::get_attribute($mcv, '$!test'), 
            undef, 
            '... we do not have the attribute yet (so we get undef)'
        );

        is(mop::internals::MopMaV::associated_class($a), undef, '... no associated class yet');

        is(SvREFCNT($a), 1, '... the attribute still has only one reference count');
        mop::internals::MopMcV::add_attribute($mcv, $a);
        is(SvREFCNT($a), 2, '... the attribute now has two reference counts');

        is(mop::internals::MopMaV::associated_class($a), $mcv, '... got associated class now');

        ok(mop::internals::MopMcV::has_attribute($mcv, '$!test'), '... we do have the attribute now');
        is(
            mop::internals::MopMcV::get_attribute($mcv, '$!test'), 
            $a, 
            '... we have the attribute and it is the same as our original'
        );
    }

}

{
    my $mcv = mop::internals::newMopMcV("Foo::Bar");

    ok(mop::internals::MopMcV::has_attribute($mcv, '$!test'), '... we do have the attribute now');
    my $sv = mop::internals::MopMcV::get_attribute($mcv, '$!test');
    is(
        SvREFCNT($sv),
        1, 
        '... the attribute only has one reference count (the original ref has been destroyed)'
    );
}

done_testing;
