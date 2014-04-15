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
	sub baz { "baz" }
}

my $m = mop::internals::newMopMmV(\&Foo::Bar::baz);

is(mop::internals::MopMmV::name($m), 'baz', '... got the right name');

my $c = mop::internals::MopMmV::associated_class($m);

is(mop::internals::MopMcV::name($c), 'Foo::Bar', '... got the associated class name correctly');
is(mop::internals::MopMcV::version($c), '0.01', '... got the associated class version correctly');

done_testing;