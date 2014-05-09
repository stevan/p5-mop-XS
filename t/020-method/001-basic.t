#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

sub foo { $_[0] + 10 }

my $old_foo_ref = \&foo . "";

{
    my $m = mop::internals::newMopMmV(\&foo);

    is(mop::internals::MopMmV::name($m), 'foo', '... got the right name');
    is(mop::internals::MopMmV::associated_class($m), \%::, '... got the right stash');

    isnt($m, $old_foo_ref, '... we have wrapped the CODE ref');
    isnt(\&foo, $old_foo_ref, '... we have stolen the GV too');
    is(\&foo, $m, '... we have stolen the GV too (doublecheck)');

    is($m->(5), 15, '... got the right value calling CODE->()');
    is(foo(20), 30, '... got the right value calling foo()');
}

# test multiple return args work 

sub bar { ($_[0] + 10, $_[0] - 5) }

my $old_bar_ref = \&foo . "";

{
    my $m = mop::internals::newMopMmV(\&bar);

    is(mop::internals::MopMmV::name($m), 'bar', '... got the right name');
    is(mop::internals::MopMmV::associated_class($m), \%::, '... got the right stash');

    isnt($m, $old_bar_ref, '... we have wrapped the CODE ref');
    isnt(\&bar, $old_bar_ref, '... we have stolen the GV too');
    is(\&bar, $m, '... we have stolen the GV too (doublecheck)');

    is_deeply([ $m->(5) ], [ 15, 0 ], '... got the right value for calling CODE->()');
    is_deeply([ bar(20) ], [ 30, 15 ], '... got the right value for calling bar()');
}

done_testing;