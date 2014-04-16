#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

my $a = mop::internals::newMopMaV('$!test');

is(mop::internals::MopMaV::name($a), '$!test', '... got the right name');

done_testing;