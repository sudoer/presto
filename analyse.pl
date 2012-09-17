
%inst=();
%func=();

$current_addr="0000";
$current_function="";
$longest_function=0;

open(LST,"<OBJ\\HBTEST.LST");
while($line=<LST>) {
   $line=~s/\012//g;
   $line=~s/\015//g;

   # read the address
   $addr=substr($line,1,4);
   if( $addr =~  m/([0-9a-fA-F]){4}/ ) {
      $current_addr=$addr;
   }

   # read the entire instruction line
   $thisdata=substr($line,23);
   $current_data=$inst{$current_addr};
   if(length($thisdata)>0) {
      $current_data.=$thisdata."\n";
   }
   $inst{$current_addr}=$current_data;

   # read the function name
   if((substr($line,23,1)eq'_')&&(substr($line,-1,1)eq':')) {
      $function=substr($line,23);
      $function=~s/^_//g;
      $function=~s/:+$//g;
      $current_function=$function;
      print("$addr $function\n");
      if(length($function)>$longest_function) {
         $longest_function=length($function);
      }
   }
   $func{$addr}=$current_function;
}
close(LST);
$longest_function++;

#for $addr (keys %inst) {
#   print("ADDRESS $addr\n");
#   print("DATA $inst{$addr}\n");
#   print("\n");
#}

# start of code
$addr="C000";
open(LOG,"<SIM68.LOG");
while($line=<LOG>) {
   if(length($inst{$addr})>0) {
      $printme=$inst{$addr};
      $spaces=' ' x $longest_function;
      $printme=~s/^/$spaces/g;
      $printme=~s/\n+$//g;
      $printme=~s/\n/\n$spaces/g;
      print("$printme\n");
   }
   print("$addr $func{$addr}");
   print(' ' x ($longest_function-length($func{$addr})-5));
   print($line);
   # save for next time
   $addr_loc=index($line,"P-");
   $addr=substr($line,$addr_loc+2,4);
}
close(LOG);
