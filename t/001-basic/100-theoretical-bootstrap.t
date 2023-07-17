#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;
use Data::Dumper qw[ Dumper ];
use Devel::Peek;

BEGIN {
    use_ok('mop')
};

use lib "t/lib";
use test::mop::bootstrap;

my ($class, $object) = ($test::mop::bootstrap::CLASS, $test::mop::bootstrap::OBJECT);

isa_ok($class, 'test::mop::class');
isa_ok($class, 'test::mop::object');

is($class->name, 'test::mop::class', '... got the correct name');
is($class->version, '0.01', '... got the correct version');
is($class->authority, 'cpan:STEVAN', '... got the correct authority');

ok($class->has_method('name'), '... name works');
ok($class->has_method('version'), '... version works');
ok($class->has_method('authority'), '... authority works');

ok($class->has_method('has_method'), '... has_method works');
ok($class->has_method('get_method'), '... get_method works');
ok($class->has_method('add_method'), '... add_method works');

ok($class->has_method('has_attribute'), '... has_attribute works');
ok($class->has_method('get_attribute'), '... get_attribute works');
ok($class->has_method('add_attribute'), '... add_attribute works');

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
    my $Foo = mop::internals::util::get_meta('test::mop::class')->construct_instance(
        mop::internals::util::get_meta('Foo')
    );

    is($Foo->name, 'Foo', '... got the expected name');
    is($Foo->version, '0.01', '... got the expected version');

    ok($Foo->has_method('bar'), '... got the method &bar');

    # regular ole stuff works too
    my $foo = Foo->new;
    isa_ok($foo, 'Foo');
    isa_ok($foo, 'test::mop::object');

    is($foo->bar, 'Foo::bar', '... instance methods');

    is(
        mop::internals::util::get_meta($foo),
        mop::internals::util::get_meta('Foo'),
        '... can get metaclass from instance or classname'
    );
}


done_testing;