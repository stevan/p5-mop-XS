#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

package Foo::Bar::Baz 0.01 {
	sub test { __PACKAGE__ . '::test' }
}

{
	my $mcv = mop::internals::newMopMcV("Foo::Bar::Baz");

	is(mop::internals::MopMcV::name($mcv), 'Foo::Bar::Baz', '... got the right name');
	is(mop::internals::MopMcV::version($mcv), '0.01', '... got the right version');
	is(mop::internals::MopMcV::authority($mcv), undef, '... got the right authority');

	is(mop::internals::MopMcV::superclass($mcv), undef, '... got the right superclass');

	my $baz = mop::internals::MopMcV::construct_instance($mcv, \(my $x));

	isa_ok($baz, 'Foo::Bar::Baz');
	can_ok($baz, 'test');

	is($baz->test, 'Foo::Bar::Baz::test', '... got the right value');
}

package Foo::Bar {
	our $VERSION   = '0.02';
	our $AUTHORITY = 'cpan:STEVAN';
	our @ISA = ('Foo::Bar::Baz');
	sub test { __PACKAGE__ . '::test' }
}

{
	my $mcv = mop::internals::newMopMcV("Foo::Bar");

	mop::internals::MopOV::set_at_slot($mcv, "$!test", 10);

	is(mop::internals::MopOV::get_at_slot($mcv, "$!test"), 10, '... got the value stored in magic');

	is(mop::internals::MopMcV::name($mcv), 'Foo::Bar', '... got the right name');
	is(mop::internals::MopMcV::version($mcv), '0.02', '... got the right version');
	is(mop::internals::MopMcV::authority($mcv), 'cpan:STEVAN', '... got the right authority');

	my $super = mop::internals::MopMcV::superclass($mcv);
	is(ref($super), 'HASH', '... this is the glob ref we expected');

	is(mop::internals::MopMcV::name($super), 'Foo::Bar::Baz', '... got the right superclass');
	is(mop::internals::MopMcV::version($super), '0.01', '... got the right version');

	my $bar = mop::internals::MopMcV::construct_instance($mcv, \(my $x));

	isa_ok($bar, 'Foo::Bar');
	isa_ok($bar, 'Foo::Bar::Baz');
	can_ok($bar, 'test');

	is($bar->test, 'Foo::Bar::test', '... got the right value');
}

# works on as yet to be created packages ...
{
	my $mcv = mop::internals::newMopMcV("Foo::Baz");

	is(mop::internals::MopMcV::name($mcv), 'Foo::Baz', '... got the right name');
	is(mop::internals::MopMcV::version($mcv), undef, '... got the right version');
	is(mop::internals::MopMcV::authority($mcv), undef, '... got the right authority');

	mop::internals::MopMcV::set_superclass($mcv, \%Foo::Bar::);

	my $super = mop::internals::MopMcV::superclass($mcv);
	is(ref($super), 'HASH', '... this is the glob ref we expected');

	is(mop::internals::MopMcV::name($super), 'Foo::Bar', '... got the right superclass');
	is(mop::internals::MopMcV::version($super), '0.02', '... got the right version');
	is(mop::internals::MopMcV::authority($super), 'cpan:STEVAN', '... got the right authority');


	my $baz = mop::internals::MopMcV::construct_instance($mcv, \(my $x));

	isa_ok($baz, 'Foo::Baz');
	isa_ok($baz, 'Foo::Bar');
	isa_ok($baz, 'Foo::Bar::Baz');
	can_ok($baz, 'test');

	is($baz->test, 'Foo::Bar::test', '... got the right value');
}

# and the magic persists
{
	my $mcv = \%Foo::Bar::;

	is(mop::internals::MopOV::get_at_slot($mcv, "$!test"), 10, '... got the (persisted) value stored in magic');

	is(mop::internals::MopMcV::name($mcv), 'Foo::Bar', '... got the right name');
	is(mop::internals::MopMcV::version($mcv), '0.02', '... got the right version');
	is(mop::internals::MopMcV::authority($mcv), 'cpan:STEVAN', '... got the right authority');
}

done_testing;