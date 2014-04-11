use v5.18;
use ExtUtils::MakeMaker;

WriteMakefile(
    NAME              => 'mop',
    VERSION_FROM      => 'lib/mop.pm',
    XSPROTOARG        => '-noprototypes',
    PREREQ_PM         => {},
    LICENSE           => 'perl',
    LIBS              => [''],
    DEFINE            => '',
    INC               => '-Isrc',
);
