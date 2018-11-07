
use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    'NAME'	=> 'MidWay',
    'VERSION_FROM' => 'MidWay.pm', # finds $VERSION
    'LIBS'	=> ['-lMidWay'],   # e.g., '-lm' 
    'DEFINE'	=> '',     # e.g., '-DHAVE_SOMETHING' 
    'OPTIMIZE'	=> '-g',     #
    'INC'	=> '-I/home/terje/projects/MidWay/include',     # e.g., '-I/usr/include/other' 
);
