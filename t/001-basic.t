#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

package test::mop::object 0.01 {
	our $AUTHORITY = 'cpan:STEVAN';

	# NOTE: simplistic hack for demo
	sub new {
		my $class = shift;
		no strict 'refs';
		(\%{$class . '::'})->construct_instance(\(my $x));
	}
}

package test::mop::class 0.01 {
	our $AUTHORITY = 'cpan:STEVAN';
	our @ISA       = ('test::mop::object');
}

my $object = mop::internals::newMopMcV("test::mop::object");
my $class  = mop::internals::newMopMcV("test::mop::class");

mop::internals::MopMcV::add_method($class, 'add_method' => \&mop::internals::MopMcV::add_method);

bless $class  => 'test::mop::class';
bless $object => 'test::mop::class';

$class->add_method('name'      => \&mop::internals::MopMcV::name);
$class->add_method('version'   => \&mop::internals::MopMcV::version);
$class->add_method('authority' => \&mop::internals::MopMcV::authority);

$class->add_method('has_method' => \&mop::internals::MopMcV::has_method);
$class->add_method('get_method' => \&mop::internals::MopMcV::get_method);

$class->add_method('construct_instance' => \&mop::internals::MopMcV::construct_instance);

isa_ok($class, 'test::mop::class');
isa_ok($class, 'test::mop::object');

is($class->name, 'test::mop::class', '... got the correct name');
is($class->version, '0.01', '... got the correct version');
is($class->authority, 'cpan:STEVAN', '... got the correct authority');

ok($class->has_method('name'), '... has_method works');
ok($class->has_method('version'), '... has_method works');
ok($class->has_method('authority'), '... has_method works');

ok($class->has_method('has_method'), '... has_method works');
ok($class->has_method('get_method'), '... has_method works');
ok($class->has_method('add_method'), '... has_method works');

isa_ok($object, 'test::mop::class');
isa_ok($object, 'test::mop::object');

is($object->name, 'test::mop::object', '... got the correct name');
is($object->version, '0.01', '... got the correct version');
is($object->authority, 'cpan:STEVAN', '... got the correct authority');

ok(!$object->has_method('name'), '... has_method only works locally');

package Foo 0.01 {
	our @ISA = ('test::mop::object');

	sub bar { 'Foo::bar' }
}

{
	# yes, this is ugly, I know, but it's
	# just a demo of creating a metaclass
	my $Foo = (\%test::mop::class::)->construct_instance(\%Foo::);

	is($Foo->name, 'Foo', '... got the expected name');
	is($Foo->version, '0.01', '... got the expected version');

	ok($Foo->has_method('bar'), '... got the method &bar');

	# regular ole stuff works too
	my $foo = Foo->new;
	isa_ok($foo, 'Foo');
	isa_ok($foo, 'test::mop::object');

	is($foo->bar, 'Foo::bar', '... instance methods');
}



done_testing;