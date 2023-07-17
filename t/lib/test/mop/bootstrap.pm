package test::mop::object 0.01 {
    use v5.18;
    use warnings;

    our $AUTHORITY = 'cpan:STEVAN';

    # NOTE: simplistic hack for demo
    sub new {
        my $class = shift;
        mop::internals::util::get_meta($class)->construct_instance(\(my $x));
    }
}

package test::mop::class 0.01 { 
    use v5.18;
    use warnings;

    our $AUTHORITY = 'cpan:STEVAN';
    our @ISA       = ('test::mop::object');

    sub name;
    sub version;
    sub authority;

    sub add_method;
    sub has_method;
    sub get_method;

    sub add_attribute;
    sub has_attribute;
    sub get_attribute;

    sub construct_instance;
}

package test::mop::bootstrap {
    use v5.18;
    use warnings;

    # these should be singletons
    our ($CLASS, $OBJECT);

    sub import {

        if (!$OBJECT) {
            $OBJECT = mop::internals::newMopMcV("test::mop::object");
            bless $OBJECT => 'test::mop::class';
        }

        if (!$CLASS) {
            $CLASS  = mop::internals::newMopMcV("test::mop::class");
            bless $CLASS  => 'test::mop::class';

            mop::internals::MopMcV::add_method($CLASS, 'add_method' => \&mop::internals::MopMcV::add_method);

            $CLASS->add_method('name'      => \&mop::internals::MopMcV::name);
            $CLASS->add_method('version'   => \&mop::internals::MopMcV::version);
            $CLASS->add_method('authority' => \&mop::internals::MopMcV::authority);

            $CLASS->add_method('has_method' => \&mop::internals::MopMcV::has_method);
            $CLASS->add_method('get_method' => \&mop::internals::MopMcV::get_method);

            $CLASS->add_method('has_attribute' => \&mop::internals::MopMcV::has_attribute);
            $CLASS->add_method('get_attribute' => \&mop::internals::MopMcV::get_attribute);
            $CLASS->add_method('add_attribute' => \&mop::internals::MopMcV::add_attribute);

            $CLASS->add_method('construct_instance' => \&mop::internals::MopMcV::construct_instance);
        }
    }
}

1;