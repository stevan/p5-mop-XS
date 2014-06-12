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
        mop::internals::util::get_meta($class)->construct_instance(\(my $x));
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

$class->add_method('has_attribute' => \&mop::internals::MopMcV::has_attribute);
$class->add_method('get_attribute' => \&mop::internals::MopMcV::get_attribute);
$class->add_method('add_attribute' => \&mop::internals::MopMcV::add_attribute);

$class->add_method('construct_instance' => \&mop::internals::MopMcV::construct_instance);

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