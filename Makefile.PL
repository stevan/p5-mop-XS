use 5.019011;
use ExtUtils::MakeMaker;

WriteMakefile(
    NAME              => 'mop',
    VERSION_FROM      => 'lib/mop.pm',
    PREREQ_PM         => {},
    LICENSE           => 'perl',
    LIBS              => [''],
    DEFINE            => '',
    INC               => '-I.',
    C                 => [ 'mop.c', 'src/p5mop.c' ],
    OBJECT            => [ 'mop.o', 'p5mop.o' ],
);
