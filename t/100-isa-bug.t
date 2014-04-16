#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

package Foo::Bar::Baz {}

#our $O = \%Foo::Bar::Baz::;

{
	#Dump($O);
	my $mcv = mop::internals::newMopMcV("Foo::Bar::Baz");
	#Dump($mcv);
}

package Foo::Bar {
	#::Dump($::O);
	our @ISA = ('Foo::Bar::Baz');
	#::Dump($::O);
}


{
	my $mcv = mop::internals::newMopMcV("Foo::Bar");

	is(mop::internals::MopMcV::superclass($mcv), 'Foo::Bar::Baz', '... got the right superclass');

	my $bar = mop::internals::MopMcV::construct_instance($mcv, \(my $x));

	isa_ok($bar, 'Foo::Bar');
	can_ok($bar, 'test');

	is($bar->test, 'Foo::Bar::test', '... got the right value');
}


done_testing;