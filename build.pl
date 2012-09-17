
################################################################################
#   B A S I C   I N C L U D E S   A N D   D E B U G   I N F O
################################################################################

# fast move/copy routines
use File::Copy;
use Cwd;

heading("PREPARING");
prepare();

heading("CONFIGURATION");
setup_project();
setup_compiler();

heading("COMPILING AND ASSEMBLING");
compile_stage();

heading("LINKING");
link_stage();

heading("SHOW THE RESULTS");
show_target_file();
generate_listing();
show_memory_usage();

exit;

################################################################################
#   C U S T O M I Z E   T H I S   P A R T
################################################################################

sub setup_project {
   print("setting up project...");
   $OBJ_DIR="obj";
   $TARGET="presto";
   @SRC_FILES=(
               "app\\stress_test.c",
               "presto\\cpu\\boot.c",
               "presto\\cpu\\crt0.s",
               "presto\\cpu\\error.c",
               "presto\\cpu\\hwtimer.c",
               "presto\\cpu\\intvect.c",
               "presto\\kernel\\clock.c",
               "presto\\kernel\\kernel.c",
               "presto\\kernel\\mail.c",
               "presto\\kernel\\memory.c",
               "presto\\kernel\\semaphore.c",
               "presto\\kernel\\timer.c",
               "services\\serial.c",
               "services\\string.c",
   );

   @INCLUDE_DIRS=("$BUILD_DIR","$BUILD_DIR\\presto",".");

   print("OK\n");
}

################################################################################

sub setup_compiler {
   print("setting up compiler...");
   $COMPILER_HOME="c:\\programs\\hc11\\gnu";
   $GNU_PREFIX="m6811-elf-";
   add_to_path("$COMPILER_HOME\\bin");
   print("OK\n");

   print("setting up linker...");
   $LIB_DIR=$COMPILER_HOME."\\lib";
   $GCC_LIB=$COMPILER_HOME."\\lib\\gcc-lib\\m6811-elf\\3.0.4\\libgcc.a";
   print("OK\n");
}

################################################################################
#   T H I S   S T U F F   S H O U L D   S T A Y   T H E   S A M E
################################################################################

sub prepare {

   # turn debug info on/off
   $DEBUG=0;
   foreach $arg (@ARGV) {
      if(tolower($arg) eq "debug") {
         $DEBUG=1;
      }
   }

   # flush after print();
   $|=1;

   # CLEAN UP OLD FILES
   print("deleting old target file...");
   unlink("$OBJ_DIR\\$TARGET.s19");
   print("OK\n");

   # CREATE OBJECT DIRECTORY
   print("creating object directory...");
   mkdir($OBJ_DIR,0777);
   print("OK\n");

   # REMEMBER WHERE YOU STARTED
   print("remembering build directory...");
   $BUILD_DIR=cwd();  # chop($BUILD_DIR=`cd`); was SLOW!
   $BUILD_DIR=~s/^[A-Za-z]://g;  # remove C:
   $BUILD_DIR=~s/\//\\/g;  # change / to \
   print("OK\n");
}

################################################################################

my %filetimes;

sub source_time {
   my ($givenfile,$prefix)=@_;
   $prefix="  ".$prefix;
   my $src_ext=extension_of($givenfile);
   my $fullpathname;
   if($src_ext eq "c") {
      $fullpathname=$givenfile;
   } elsif($src_ext eq "h") {
      # find the H file in the include path
      $fullpathname=find_include_file_in_path($givenfile);
      if(length($fullpathname)==0) {
         # H file not found in include path
         debug("ERROR! H file (".$givenfile.") not found");
         return 0;
      }
   } elsif($src_ext eq "s") {
      $fullpathname=$givenfile;
   } else {
      # not a C or H file
      debug("ERROR! unrecognized file type");
      return 0;
   }

   #debug($prefix."SOURCE TIME OF [$fullpathname]");

   if(!defined($filetimes{$fullpathname})) {
      $filetimes{$fullpathname}= -M $fullpathname;
      #debug($prefix."filetime{$fullpathname}=<".$filetimes{$fullpathname}.">");
      my @all_includes=determine_source_dependencies($fullpathname);
      my $one_include;
      foreach $one_include (@all_includes) {
         my $st=source_time($one_include,$prefix);
         if($st<$filetimes{$fullpathname}) {
            $filetimes{$fullpathname}=$st;
            #debug($prefix."[$one_include] is newer than [$fullpathname]");
         }
      }
   }

   return $filetimes{$fullpathname};
}

################################################################################

sub determine_source_dependencies {
   my $source_file=$_[0];
   my @includes=();
   open(SRCFILE,"<$source_file");
   my $line;
   while($line=<SRCFILE>) {
      $line=~s/\012//g;
      $line=~s/\015//g;
      $line=~s/\/\/.*//g;  # remove // comments
      $line=~s/\//\\/g;  # change / to \
      if(index($line,"#include")>-1) {
         $match=$line;
         $match=~s/^ *#include +\"//g;
         $match=~s/\" *$//g;
         push(@includes,$match);
      }
   }
   close(SRCFILE);
   return @includes;
}

################################################################################

sub find_include_file_in_path {
   my $h_file=$_[0];
   my $dir;
   foreach $dir (@INCLUDE_DIRS) {
      if($dir eq ".") {
         $dir=cwd();
      }
      $dir=~s/\//\\/g;  # change / to \
      if(-f "$dir\\$h_file") {
         #debug("INCLUDE FILE [$h_file] is [$dir\\$h_file]");
         return "$dir\\$h_file";
      }
   }
   return "";
}

################################################################################

sub compile_stage {

   @OBJS=();
   $TOTAL_ERRORS=0;

   # BUILD INCLUDE PATH (to pass to compiler)

   my $include_path="";
   foreach $dir (@INCLUDE_DIRS) {
      $include_path=$include_path."-I$dir "
   }

   # LOOPING THROUGH OBJECT FILES

   foreach $src_file (@SRC_FILES) {
      $errors=0;

      # DETERMINE FILE NAMES, BASE NAMES, EXTENSIONS, PATHS

      $src_path="$BUILD_DIR\\$src_file";
      $src_dir=directory_of($src_path);
      $src_name=basename($src_path);
      $src_base=chop_extension($src_name);
      $src_ext=extension_of($src_path);
      debug("");
      #debug("FILE $src_path");
      #debug("src_name=[$src_name]");
      #debug("src_path=[$src_path]");
      #debug("src_dir=[$src_dir]");
      #debug("src_base=[$src_base]");
      #debug("src_ext=[$src_ext]");

      $obj_name="$src_base.o";
      $obj_path="$BUILD_DIR\\$OBJ_DIR\\$obj_name";
      #debug("obj_name=[$obj_name]");
      #debug("obj_path=[$obj_path]");
      $lst_path="$BUILD_DIR\\$OBJ_DIR\\$src_base.lst";

      # DETERMINE IF WE NEED TO DO ANYTHING FOR THIS FILE

      chdir($src_dir);

      $work=0;
      if( ! -e "$obj_path" ) {
         $work=1;
      } else {
         $filetime=source_time($src_path);
         $objtime= -M $obj_path;
         if( $filetime < $objtime ) {
            #debug("{$src_path} is newer than ($obj_path}");
            #debug("{$filetime} is newer than ($objtime}");
            $work=1;
         }
      }

      # IF WE HAVE WORK TO DO, THEN DO IT

      if( $work == 1 ) {
         if($src_ext eq "c") {
            print("COMPILING  $src_file...");
            debug("");
            $errors+=run($GNU_PREFIX."gcc.exe "      # compiler
                        ."-m68hc11 "                 # platform, hc11 or hc12
                        ."-DGCC "                    # define GCC
                        .$include_path               # include path
                        ."-mshort "                  # ???
                        ."-O1 "                      # optimization, was (oh-zero)
                        ."-fomit-frame-pointer "     # ???
                        ."-msoft-reg-count=0 "       # ???
                        ."-funsigned-char "          # ???
                        ."-c "                       # compile only, do not link
                        ."-g "                       # include debug info
                        ."-Wall "                    # enable all warnings
                       #."-Werror "                  # treat warnings as errors
                        ."-Wa,-L,-ahlns=$lst_path "  # generate list file
                        ."-o $obj_path "             # output file
                        ."$src_name");               # source file
            print("OK\n");
         } elsif($src_ext eq "s") {
            print("ASSEMBLING $src_file...");
            debug("");
            $errors+=run($GNU_PREFIX."as.exe "       # assembler
                        ."-L "                       # include local symbols in debug table
                        ."-ahlns=$lst_path "         # generate list file
                        ."-o $obj_path "             # output file
                        ."$src_name");               # source file
            print("OK\n");
         } else {
            print("WHAT???    $src_file\n");
         }
      } else {
         print("NO WORK ON $src_file\n");
         debug("");
      }

      # RECORD THE FILES THAT WE NEED TO LINK TOGETHER

      push(@OBJS,$obj_name);

      $TOTAL_ERRORS+=$errors;
      if($errors > 0) {
         print("\n");
         print("ERRORS IN COMPILE ... stopping\n");
         return;
      }

      chdir($BUILD_DIR);
   }

}

################################################################################

sub link_stage {

   if($TOTAL_ERRORS > 0) {
      print("errors compiling, skipping the link stage\n");
      print("\n");
      return;
   }

   chdir("$BUILD_DIR\\$OBJ_DIR");

   $ofiles="";
   foreach $obj_file (@OBJS) {
      $ofiles=$ofiles." ".$obj_file;
   }

   my $trace="";
   if($DEBUG) { $trace="--trace"; }

   print("LINKING...\n");
   $errors+=run($GNU_PREFIX."ld.exe "   # linker
      ."--script ..\\memory.x "         # memory and segment map
      ."-m m68hc11elf "                 # architecture, file format
      ."$trace "                        # print file names as they are completed
      ."-nostdlib "                     # do not include C standard library
      ."-nostartfiles "                 # do not include crt0.s
      ."-defsym _.tmp=0x0 "             # temporary "soft" register
      ."-defsym _.z=0x2 "               # temporary "soft" register
      ."-defsym _.xy=0x4 "              # temporary "soft" register
      ."-Map $TARGET.map --cref "       # create memory map file
      ."--oformat=elf32-m68hc11 "       # output file format
      ."-o $TARGET.elf "                # output file
      ."$ofiles "                       # input files
      ."$GCC_LIB");                     # include GCC library
      #."-L$LIB_DIR -lgcc");             # include GCC library (should be last argument)
   print("OK\n");
   print("\n");


   if($errors==0) {
      print("CONVERTING...\n");
      $errors+=run($GNU_PREFIX."objcopy.exe "
         ."--output-target=srec "
         #."--srec-forceS3 "
         ."--strip-all "
         ."--strip-debug "
         ."$TARGET.elf $TARGET.s19");
      print("OK\n");
      print("\n");
   }


   $TOTAL_ERRORS+=$errors;
   chdir($BUILD_DIR);
}

################################################################################

sub show_target_file {

   if($TOTAL_ERRORS > 0) {
      return;
   }

   my $tempfile="$TARGET.dir";
   run("dir $OBJ_DIR\\$TARGET.S19 > $tempfile");

   open(TEMP,"<$tempfile");
   my $line;
   while($line=<TEMP>) {
      if(index($line,$TARGET)>-1) {
         print($line);
      }
   }
   close(TEMP);
   unlink($tempfile);
   print("\n");
}

################################################################################

sub generate_listing {

   if($TOTAL_ERRORS > 0) {
      return;
   }

   chdir("$BUILD_DIR\\$OBJ_DIR");

   print("GENERATING LISTING...\n");
   $errors+=run($GNU_PREFIX."objdump.exe "
      ."--disassemble-all "
      ."--architecture=m68hc11 "
      ."--section=.text "
      #."--debugging "
      ."$TARGET.elf > $TARGET.lst");
   print("OK\n");
   print("\n");

   $TOTAL_ERRORS+=$errors;
   chdir($BUILD_DIR);
}

################################################################################

sub show_memory_usage {

   if($TOTAL_ERRORS > 0) {
      return;
   }

   print("MEMORY USAGE\n");
   print("------------\n");

   my $tempfile="headers.txt";  #$TARGET.".tmp";
   run($GNU_PREFIX."objdump.exe -h $OBJ_DIR\\$TARGET.elf > $tempfile");

   my %memusage_sectsize;
   my @memusage_ramsections;
   my @memusage_romsections;
   my %memusage_used;

   open(TEMP,"<$tempfile");
   my $line;
   while($line=<TEMP>) {
      if(index($line,"2**0")>-1) {
         # combine with next line
         $line=$line.<TEMP>;
         $line=~s/\012//g;
         $line=~s/\015//g;
      }

      if(index($line,"2**0")>-1) {
         my $section_name=substr($line,5,12);
         $section_name=~s/ *$//g;
         my $size=hex(substr($line,18,8));
         my ($memory,$section)=section_info($section_name);

         $memusage_sectsize{$section}+=$size;

         if((index($memory,"ROM")>-1)&&(!defined($memusage_used{"ROM-".$section}))) {
            push(@memusage_romsections,$section);
            $memusage_used{"ROM-".$section}=1;
         }

         if((index($memory,"RAM")>-1)&&(!defined($memusage_used{"RAM-".$section}))) {
            push(@memusage_ramsections,$section);
            $memusage_used{"RAM-".$section}=1;
         }

      }

   }
   close(TEMP);
   unlink($tempfile);

   my $rom_space=0;
   my $ram_space=0;
   foreach $section (sort @memusage_romsections) {
      printf("ROM  %-15s %d\n",$section,$memusage_sectsize{$section});
      $rom_space+=$memusage_sectsize{$section};
   }
   foreach $section (sort @memusage_ramsections) {
      printf("RAM  %-15s %d\n",$section,$memusage_sectsize{$section});
      $ram_space+=$memusage_sectsize{$section};
   }
   printf("totals: ROM=%d, RAM=%d\n",$rom_space,$ram_space);
   print("\n");
}

################################################################################

sub section_info {
   my $header=$_[0];
   if(     $header eq "text") {       return ( "ROM"    , "code"           );
   } elsif($header eq "rodata") {     return ( "ROM"    , "const data"     );
   } elsif($header eq "strings") {    return ( "ROM"    , "strings"        );
   } elsif($header eq "specvect") {   return ( "ROM"    , "vectors (init)" );
   } elsif($header eq "data") {       return ( "ROM,RAM", "data (init)"    );
   } elsif($header eq "bss") {        return (     "RAM", "data (zero)"    );
   } elsif($header eq "common") {     return (     "RAM", "data (uninit)"  );
   } elsif($header eq "normvect") {   return (     "RAM", "vectors (zero)" );
   } elsif($header eq "stack") {      return (     "RAM", "stack (zero)"   );
   } elsif($header eq "heap") {       return (     "RAM", "heap (zero)"    );
   } elsif($header eq "end_of_rom") { return ( ""       , "???"            );
   } elsif($header eq "softregs") {   return ( ""       , "page0"          );
   } elsif($header eq "comment") {    return ( ""       , "debug"          );
   } elsif($header eq "debug") {      return ( ""       , "debug"          );
   } else {                           return ( ""       , "???"            );
   }
}

################################################################################
#   F U N C T I O N S
################################################################################

sub heading {
   my $string=$_[0];
   if($TOTAL_ERRORS > 0) {
      return;
   }
   my $text_width=60;
   $string=~s/\n//g;
   $string=~s/^ +//g;
   my $count=($text_width-length($string)-2)/2;
   my $spaces= "-" x $count;
   print("\n");
   print("$spaces $string $spaces\n");
   print("\n");
}

################################################################################

sub debug {
   my $string=$_[0];
   my $prefix="   ";
   if(length($string)==0) {
      $prefix="";
   }
   if($DEBUG) {
      print($prefix.$string."\n");
   }
}

################################################################################

# search_and_replace(file,search1,replace1,search2,replace2,...)
sub search_and_replace {
   my @args=@_;
   my $filename=shift @args;
   my $count;
   my $changes;
   my @search=();
   my @replace=();
   my $replacements_made;
   my $tempfile="$filename.$$";

   $changes=0;
   while(($changes*2)<$#args) {
      $search[$changes]=@args[$changes*2];
      $replace[$changes]=@args[$changes*2+1];
      $changes++;
   }

   if($DEBUG) {
      for $count (0..$changes-1) {
         print("replace [$search[$count]] with [$replace[$count]]\n");
      }
   }

   open(OLD,$filename);
   open(NEW,">$tempfile");
   while($line=<OLD>) {
      $replacements_made=0;
      for $count (0..$changes-1) {
         while($find=index($line,$search[$count])>-1) {
            substr($line,$find,length($search[$count]))=$replace[$count];
            $replacements_made++;
         }
      }
      if(($DEBUG>0)&&($replacements_made>0)) {
         print("CHANGED $line");
      }
      print NEW $line;
   }
   close(OLD);
   close(NEW);
   unlink $filename;
   move($tempfile,$filename);
}

################################################################################

sub run {
   my $cmd=$_[0];
   my $rc;
   if($DEBUG) {
      print("RUNNING [$cmd]\n\n");
   }
   $rc=system($cmd);
   return $rc;
}

################################################################################

sub move_file {
   my $src=$_[0];
   my $dest=$_[1];
   if($DEBUG) {
      print("MOVING [$src] to [$dest]\n");
   }
   move($src,$dest);
}

################################################################################

sub unix_slashes {
   my $string=$_[0];
   $string=~s/\\/\//g;
   return $string;
}

################################################################################

sub tolower {
   my $string=$_[0];
   $string=~tr/A-Z/a-z/;
   return $string;
}

################################################################################

sub toupper {
   my $string=$_[0];
   $string=~tr/a-z/A-Z/;
   return $string;
}

################################################################################

sub setenv {
   my $varname=$_[0];
   my $value=$_[1];
   $ENV{$varname}=$value;
   #print("$varname=[$value]\n");;
}

################################################################################

sub add_to_path {
   my $dir=tolower($_[0]);
   my $path=tolower($ENV{"PATH"});
   if(index($dir,$path)<0) {
      setenv("PATH",$ENV{"PATH"}.";$dir");
   }
}

################################################################################

sub chop_extension {
   my $filename=tolower($_[0]);
   $filename=~s/\..*$//g;
   return $filename;
}

################################################################################

sub add_extension {
   my $filename=tolower($_[0]);
   my $ext=tolower($_[1]);
   $filename="$src_name.$ext";
   return $filename;
}

################################################################################

sub directory_of {
   my $dir=tolower($_[0]);
   # convert to lowercase
   $dir=~tr/A-Z/a-z/;
   # remove C:
   $dir=~s/^[a-z]://g;
   # remove stuff after last backslash
   $dir=~s/\\[^\\]*$//g;
   return $dir;
}

################################################################################

sub extension_of {
   my $ext=tolower($_[0]);
   # convert to lowercase
   $ext=~tr/A-Z/a-z/;
   $ext=~s/^.*\.//g;
   return $ext;
}

################################################################################

sub basename {
   my $filename=tolower($_[0]);
   # get rid of everything before backslashes
   $filename=~s/^.*\\//g;
   # get rid of everything before slashes
   $filename=~s/^.*\///g;
   return $filename;
}

################################################################################

sub dec2hex {
   return sprintf("%x",$_[0]);
}

################################################################################


