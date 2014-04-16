#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

package Foo {
	sub test_foo { "FOO" }
}

# uncomment this line and no more OOM error
#our $O = \%Foo::;

{
	my $mcv = mop::internals::newMopMcV("Foo");
}

package Foo::Bar {
	our @ISA = ('Foo');
	sub test_bar { "FOOBAR" }
}

{
	my $mcv = mop::internals::newMopMcV("Foo::Bar");

	is(mop::internals::MopMcV::superclass($mcv), 'Foo', '... got the right superclass');

	my $bar = mop::internals::MopMcV::construct_instance($mcv, \(my $x));

	isa_ok($bar, 'Foo::Bar');
	isa_ok($bar, 'Foo');

	can_ok($bar, 'test_bar');
	can_ok($bar, 'test_foo');

	ok($bar->isa('Foo::Bar'), '... $bar isa Foo::Bar');
	ok($bar->isa('Foo'), '... $bar isa Foo');

	ok($bar->can('test_foo'), '... $bar can call method test_foo');
	ok($bar->can('test_bar'), '... $bar can call method test_bar');

	is($bar->test_foo, 'FOO', '... got the right value for foo');
	is($bar->test_bar, 'FOOBAR', '... got the right value for foobar');
}


done_testing;