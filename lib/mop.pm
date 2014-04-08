package mop;

use v5.19;
use warnings;

our $VERSION   = '0.01';
our $AUTHORITY = 'cpan:STEVAN';

require XSLoader;
XSLoader::load('mop', $VERSION);

1;

__END__
