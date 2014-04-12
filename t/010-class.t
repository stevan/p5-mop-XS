#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;
use B qw[ svref_2object ];

BEGIN {
    use_ok('mop')
};

package Foo::Bar::Baz 0.01 {
	sub test {}
}

my $mcv = mop::internals::newMopMCV("Foo::Bar::Baz");

is(mop::internals::MopMCV::name($mcv), 'Foo::Bar::Baz', '... got the right name');
is(mop::internals::MopMCV::version($mcv), '0.01', '... got the right version');
is(mop::internals::MopMCV::authority($mcv), undef, '... got the right authority');


done_testing;