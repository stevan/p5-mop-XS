#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

package Foo::Bar {
	sub baz { "baz" }
}

my $method = \&Foo::Bar::baz;

my $m = mop::internals::newMopMmV($method);

is(mop::internals::MopMmV::name($m), 'baz', '... got the right name');

my $c = mop::internals::MopMmV::associated_class($m);

is(mop::internals::MopMcV::name($c), 'Foo::Bar', '... got the associated class name correctly');

done_testing;