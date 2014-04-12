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

{
	my $mcv = mop::internals::newMopMcV("Foo::Bar::Baz");

	is(mop::internals::MopMcV::name($mcv), 'Foo::Bar::Baz', '... got the right name');
	is(mop::internals::MopMcV::version($mcv), '0.01', '... got the right version');
	is(mop::internals::MopMcV::authority($mcv), undef, '... got the right authority');
}

package Foo::Bar {
	our $VERSION   = '0.02';
	our $AUTHORITY = 'cpan:STEVAN';
	sub test {}
}

{
	my $mcv = mop::internals::newMopMcV("Foo::Bar");

	mop::internals::MopOV::set_at_slot($mcv, "$!test", 10);

	is(mop::internals::MopOV::get_at_slot($mcv, "$!test"), 10, '... got the value stored in magic');

	is(mop::internals::MopMcV::name($mcv), 'Foo::Bar', '... got the right name');
	is(mop::internals::MopMcV::version($mcv), '0.02', '... got the right version');
	is(mop::internals::MopMcV::authority($mcv), 'cpan:STEVAN', '... got the right authority');
}

# works on as yet to be created packages ...
{
	my $mcv = mop::internals::newMopMcV("Foo::Baz");

	is(mop::internals::MopMcV::name($mcv), 'Foo::Baz', '... got the right name');
	is(mop::internals::MopMcV::version($mcv), undef, '... got the right version');
	is(mop::internals::MopMcV::authority($mcv), undef, '... got the right authority');
}

# and the magic persists 
{
	no strict 'refs';
	my $mcv = \%{"Foo::Bar::"};

	is(mop::internals::MopOV::get_at_slot($mcv, "$!test"), 10, '... got the (persisted) value stored in magic');

	is(mop::internals::MopMcV::name($mcv), 'Foo::Bar', '... got the right name');
	is(mop::internals::MopMcV::version($mcv), '0.02', '... got the right version');
	is(mop::internals::MopMcV::authority($mcv), 'cpan:STEVAN', '... got the right authority');
}


done_testing;