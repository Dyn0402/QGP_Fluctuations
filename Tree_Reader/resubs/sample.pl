#! /opt/star/bin/perl -w

my $path = "//gpfs01/star/pwg/dneff/scratch/trees/script/62GeV/";
my $sufx = "sched*_*_*.condor";

#foreach $eachCent (@flowCents) {  
  foreach(glob ("$path$sufx"))
  {
    my $eachScript = $_;
    open (prototypeMacro, "$eachScript");
    chomp $eachScript;
    @fields = split(/\//,$eachScript) ;
    $eachScriptNoPath= $fields[$#fields];
    ($eachJOBID = $eachScriptNoPath) =~ s/\.condor// ;
    chomp $eachJOBID;

    @names = split(/_/,$eachJOBID) ;
    $eachNumber= $names[$#names];
    $eachNumberLow = $names[$#names-1];
    $Number = $eachNumber - $eachNumberLow + 1;
    $eachName = $names[0];
	print "$eachNumberLow\t";
  	print "$eachNumber\t";
	print "$Number\t";
  	print "$eachName\n";

    $count =0.;
    while ($eachLine = <prototypeMacro>) {

      if(($count % 16)==0) {open (macroFile,">$path$eachName\_$eachNumber.condor");}

      print macroFile $eachLine;
      $count = $count+1;

      if(($count % 16)==0) {close macroFile; $eachNumber = $eachNumber -1;}
    }
    
     close prototypeMacro;
  }
#}

