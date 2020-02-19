#!/usr/apps/bin/perl
#
# perl program to loop over various Makefile options
# run with : perl timing2D.perl

#
# Here is the template makefile and the output file
$cmdFile="./Makefile_template";
$outFile="./Makefile_timing2D";

# Flags to test
@array_flags = ("-g -O0 -DBZ_DEBUG", "-O3 -DBZ_DEBUG", "-O3");

for ( $m = 0; $m < 3; $m = $m+1) {
  # Open the Template file and the output file.
  open(FILE,"$cmdFile") || die "cannot open file $cmdFile!" ;
  open(OUTFILE,"> $outFile") || die "cannot open file!" ;
  # Setup the outfile based on the template
  # read one line at a time.
  while ( $line = <FILE> ) {
    # Replace the the stings by using substitution
    # s
    $line =~ s/\bFFFFFF\b/$array_flags[$m]/;
    print OUTFILE $line;
    # Note, you can always print to secreen to see what is happening.
    # Uncomment next line to do so.
    # print $line;
  }
  # Close the files
  close( OUTFILE );
  close( FILE );

  # Run the shell commands to compile and run the program
  system("make -s clean; make -f Makefile_timing2D");
  $flag = $array_flags[$m];
  print "\n \n Running tests with the flag: " . $flag ." \n \n";
  system("./ex.x ");
}

exit
