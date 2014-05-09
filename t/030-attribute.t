#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

package Foo::Bar::Baz 0.01 {}

{
    my $a = mop::internals::newMopMaV('$!test');

    is(mop::internals::MopMaV::name($a), '$!test', '... got the right name');
    is(mop::internals::MopMaV::key_name($a), 'test', '... got the right key name');
    is(mop::internals::MopMaV::name($a), '$!test', '... still got the right name');
}

done_testing;