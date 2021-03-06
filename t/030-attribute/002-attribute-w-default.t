#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek qw[ Dump SvREFCNT ];

BEGIN {
    use_ok('mop')
};

my $x = 0;
sub default_generator { ++$x }

{
    my $a = mop::internals::newMopMaV('$!test');

    is(mop::internals::MopMaV::name($a), '$!test', '... got the right name');
    is(mop::internals::MopMaV::key_name($a), 'test', '... got the right key name');
    is(mop::internals::MopMaV::name($a), '$!test', '... still got the right name');

    ok(!mop::internals::MopMaV::has_default_generator($a), '... we do not have a default value');
    is(mop::internals::MopMaV::get_default_generator($a), undef, '... we get undef instead of default value');

    mop::internals::MopMaV::set_default_generator($a, \&default_generator);

    ok(mop::internals::MopMaV::has_default_generator($a), '... we do have a default value now');
    is(mop::internals::MopMaV::get_default_generator($a), \&default_generator, '... we get the expected default value');

    is(mop::internals::MopMaV::get_default_generator($a)->(), 1, '... got the new default value');
    is(mop::internals::MopMaV::get_default_generator($a)->(), 2, '... got the new default value');

    mop::internals::MopMaV::clear_default_generator($a);

    ok(!mop::internals::MopMaV::has_default_generator($a), '... we do not have a default value anymore');
    is(mop::internals::MopMaV::get_default_generator($a), undef, '... back to getting undef instead of default value');
}

done_testing;