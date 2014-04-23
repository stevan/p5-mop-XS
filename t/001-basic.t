#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;

BEGIN {
    use_ok('mop')
};

package mop::object 0.01 {
	our $AUTHORITY = 'cpan:STEVAN';
}

package mop::class 0.01 {
	our $AUTHORITY = 'cpan:STEVAN';
	our @ISA       = ('mop::object');
}

my $object = mop::internals::newMopMcV("mop::object");
my $class  = mop::internals::newMopMcV("mop::class");

mop::internals::MopMcV::add_method($class, 'add_method' => \&mop::internals::MopMcV::add_method);

bless $class  => 'mop::class';
bless $object => 'mop::class';

$class->add_method('name'      => \&mop::internals::MopMcV::name);
$class->add_method('version'   => \&mop::internals::MopMcV::version);
$class->add_method('authority' => \&mop::internals::MopMcV::authority);

$class->add_method('has_method' => \&mop::internals::MopMcV::has_method);
$class->add_method('get_method' => \&mop::internals::MopMcV::get_method);

isa_ok($class, 'mop::class');
isa_ok($class, 'mop::object');

is($class->name, 'mop::class', '... got the correct name');
is($class->version, '0.01', '... got the correct version');
is($class->authority, 'cpan:STEVAN', '... got the correct authority');

ok($class->has_method('name'), '... has_method works');
ok($class->has_method('version'), '... has_method works');
ok($class->has_method('authority'), '... has_method works');

ok($class->has_method('has_method'), '... has_method works');
ok($class->has_method('get_method'), '... has_method works');
ok($class->has_method('add_method'), '... has_method works');

isa_ok($object, 'mop::class');
isa_ok($object, 'mop::object');

is($object->name, 'mop::object', '... got the correct name');
is($object->version, '0.01', '... got the correct version');
is($object->authority, 'cpan:STEVAN', '... got the correct authority');

ok(!$object->has_method('name'), '... has_method only works locally');

done_testing;