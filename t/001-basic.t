#!/usr/bin/env perl

use v5.18;
use warnings;

use Test::More;

BEGIN {
    use_ok('mop')
};


package mop::class {

}

package mop::object {

}

#my $object = mop::internals


done_testing;