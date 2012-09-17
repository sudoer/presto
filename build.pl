
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

heading("PRE-BUILD CLEANING");
prebuild_cleaning();

heading("COMPILING AND ASSEMBLING");
compile_stage();

heading("LINKING");
link_stage();
convert_binary();

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

   $TARGET="presto";

   $OBJ_DIR=""
      .ifcpu($CPU_M68HC11,"obj\\m68hc11")
      .ifcpu($CPU_AVR8515,"obj\\avr8515")
      ;

   my $cpu_dir=""
      .ifcpu($CPU_M68HC11,"cpu\\m68hc11")
      .ifcpu($CPU_AVR8515,"cpu\\avr8515")
      ;

   @SRC_FILES=(

      # APPLICATION FILES

      ifcpu($CPU_M68HC11,"app\\main.c"),
      ifcpu($CPU_M68HC11,"app\\control.c"),
      ifcpu($CPU_M68HC11,"app\\president.c"),
      ifcpu($CPU_M68HC11,"app\\manager.c"),
      ifcpu($CPU_M68HC11,"app\\employee.c"),
      ifcpu($CPU_M68HC11,"app\\student.c"),
      ifcpu($CPU_M68HC11,"app\\shared.c"),
      ifcpu($CPU_M68HC11,"app\\debug.c"),
      ifcpu($CPU_M68HC11,"services\\serial.c"),
      ifcpu($CPU_M68HC11,"services\\string.c"),

      ifcpu($CPU_AVR8515,"app\\small_test.c"),

      # CORE RTOS KERNEL

      "presto\\kernel\\clock.c",
      "presto\\kernel\\memory.c",
      "presto\\kernel\\semaphore.c",
      "presto\\kernel\\timer.c",
      "presto\\kernel\\mail.c",
      "presto\\kernel\\kernel.c",

      # CPU SUPPORT FILES

      "$cpu_dir\\boot.c",
      "$cpu_dir\\error.c",
      "$cpu_dir\\cpu_timer.c",
      "$cpu_dir\\vectors.c",

      ifcpu($CPU_M68HC11,"$cpu_dir\\crt0.s"),

   );

   @INCLUDE_DIRS=(
      "$BUILD_DIR",
      "$BUILD_DIR\\app",
      "$BUILD_DIR\\presto",
      "$BUILD_DIR\\$cpu_dir",
      "."
   );

   print("OK\n");
}

################################################################################

sub setup_compiler {

   print("setting up compiler...");
   $COMPILER_HOME=""
      .ifcpu($CPU_M68HC11,"c:\\programs\\hc11\\gnu")
      .ifcpu($CPU_AVR8515,"c:\\programs\\avr\\gnu")
      ;
   $GNU_PREFIX=""
      .ifcpu($CPU_M68HC11,"m6811-elf-")
      .ifcpu($CPU_AVR8515,"avr-")
      ;
   add_to_path("$COMPILER_HOME\\bin");
   print("OK\n");

   print("setting up linker...");
   #$LIB_DIR=$COMPILER_HOME
   #   .ifcpu($CPU_M68HC11,"\\m6811-elf\\lib")
   #   .ifcpu($CPU_AVR8515,"\\avr\\lib")
   #   ;
   $GCC_LIB=$COMPILER_HOME
      .ifcpu($CPU_M68HC11,"\\lib\\gcc-lib\\m6811-elf\\3.0.4\\libgcc.a")
      .ifcpu($CPU_AVR8515,"\\lib\\gcc-lib\\avr\\3.3\\libgcc.a")
      ;
   print("OK\n");
}

################################################################################
#   T H I S   S T U F F   S H O U L D   S T A Y   T H E   S A M E
################################################################################

sub prepare {

   # flush after print();
   $|=1;

   # DEFINE CPU ARCHITECTURES
   print("defining architectures...");
   $CPU_M68HC11=1;
   $CPU_AVR8515=2;
   print("OK\n");

   # turn debug info on/off
   print("parsing command-line arguments...");
   $DEBUG=0;
   foreach $arg (@ARGV) {
      if(tolower($arg) eq "debug") {
         $DEBUG=1;
      }
      if(tolower($arg) eq "m68hc11") {
         $CPU=$CPU_M68HC11;
      }
      if(tolower($arg) eq "avr8515") {
         $CPU=$CPU_AVR8515;
      }
   }
   print("OK\n");

   if(!defined($CPU)) {
      print("MUST SPECIFY TARGET:\n");
      print(" - M68HC11\n");
      print(" - AVR8515\n");
      print("\n");
      print("usage: PERL BUILD.PL <target> [DEBUG]\n");
      exit(1);
   }

   # REMEMBER WHERE YOU STARTED
   print("remembering build directory...");
   $BUILD_DIR=cwd();  # chop($BUILD_DIR=`cd`); was SLOW!
   $BUILD_DIR=~s/^[A-Za-z]://g;  # remove C:
   $BUILD_DIR=~s/\//\\/g;  # change / to \
   print("OK\n");

}

################################################################################

sub prebuild_cleaning {

   # CREATE OBJECT DIRECTORY
   print("creating object directory...");
   debug("");
   make_directory($OBJ_DIR);
   print("OK\n");

   # CLEAN UP OLD FILES
   print("deleting old target file...");
   unlink("$OBJ_DIR\\$TARGET.s19");
   print("OK\n");
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

   # remove blank entries from source file list
   my @temp_array=();
   for $count (0..$#SRC_FILES) {
      if(length($SRC_FILES[$count])>0) {
         push(@temp_array,$SRC_FILES[$count]);
      }
   }
   @SRC_FILES=@temp_array;

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

            my $gcc_options=""

               # TELL MY PROGRAMS WHAT CPU WE'RE USING
               .ifcpu($CPU_M68HC11,"-DCPU_M68HC11 ")      # 
               .ifcpu($CPU_AVR8515,"-DCPU_AVR8515 ")      # 

               # DEFINE ARCHITECTURE
               .ifcpu($CPU_M68HC11,"-m68hc11 ")           # platform, hc11 or hc12
               .ifcpu($CPU_AVR8515,"-mmcu=at90s8515 ")    # platform, which AVR

               # WHERE TO FIND INCLUDE FILES
               .$include_path                             # include path

               # OPTIMIZATION
               .ifcpu($CPU_M68HC11,"-Os ")                # optimization, was (oh-zero)
               .ifcpu($CPU_AVR8515,"-O1 ")                # optimization = (oh-zero)
               ."-fomit-frame-pointer "                   # when possible do not generate stack frames

               # SIZES OF TYPES
               ."-funsigned-char "                        # chars are unsigned

               # M68HC11-SPECIFIC OPTIONS
               .ifcpu($CPU_M68HC11,"-mshort ")            # use short ints
               .ifcpu($CPU_M68HC11,"-msoft-reg-count=0 ") # soft registers available

               # STORAGE
               ."-fwritable-strings "                     # store strings in "strings" section, not inline

               # WARNINGS AND ERRORS
               ."-Wall "                                  # enable all warnings
               #."-Werror "                               # treat warnings as errors

               # LISTINGS
               ."-Wa,-L,-ahlns=$lst_path "                # inc local sym, generate list file

               # WHAT TO DO
               ."-c "                                     # compile only, do not link
               ."-g "                                     # include debug info
               ;

            $errors+=run($GNU_PREFIX."gcc.exe $gcc_options -o $obj_path $src_name");
            print("OK\n");
         } elsif($src_ext eq "s") {
            print("ASSEMBLING $src_file...");
            debug("");
            $errors+=run($GNU_PREFIX."as.exe "
               #."-x assembler-with-cpp "        # used with gcc, not with as
               ."-L "                            # include local symbols in debug table
               ."-ahlns=$lst_path "              # generate list file
               ."-o $obj_path "                  # output file
               ."$src_name");                    # source file
            print("OK\n");
         }
      } else {
         print("NO WORK ON $src_file\n");
         debug("");
      }

      $TOTAL_ERRORS+=$errors;
      if($errors > 0) {
         print("\n");
         print("ERRORS IN COMPILE ... stopping\n");
         return;
      }

      # record the files that we need to link together
      push(@OBJS,$obj_name);

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


   $errors+=run($GNU_PREFIX."ld.exe "

      # DEFINE ARCHITECTURE
      .ifcpu($CPU_M68HC11,"-m m68hc11elf ")            # architecture, file format
      .ifcpu($CPU_AVR8515,"-m avr85xx ")               # architecture, file format

      # DEFINE MEMORY MAP
      .ifcpu($CPU_M68HC11,"--script $BUILD_DIR\\cpu\\m68hc11.x ")
      .ifcpu($CPU_AVR8515,"--script $BUILD_DIR\\cpu\\avr8515.x ")

      # GIVE FEEDBACK ON THE SCREEN, IN MAP FILE
      ."$trace "                                       # print file names as they are completed
      ."-Map $TARGET.map --cref "                      # create memory map file

      # M68HC11-SPECIFIC STUFF
      .ifcpu($CPU_M68HC11,"-nostdlib ")                # do not include C standard library
      .ifcpu($CPU_M68HC11,"-nostartfiles ")            # do not include crt0.s
      .ifcpu($CPU_M68HC11,"-defsym _.tmp=0x0 ")        # temporary "soft" register
      .ifcpu($CPU_M68HC11,"-defsym _.z=0x2 ")          # temporary "soft" register
      .ifcpu($CPU_M68HC11,"-defsym _.xy=0x4 ")         # temporary "soft" register
      .ifcpu($CPU_M68HC11,"--oformat=elf32-m68hc11 ")  # output file format

      # AVR-SPECIFIC STUFF
      .ifcpu($CPU_AVR8515,"-nostdlib ")                # do not include C standard library

      # OUTPUT FILE
      ."-o $TARGET.elf "

      # INPUT FILES
      ."$ofiles "
      .ifcpu($CPU_AVR8515,"$COMPILER_HOME\\avr\\lib\\crts8515.o ")
      ."$GCC_LIB "
   );

   print("OK\n");
   print("\n");

   $TOTAL_ERRORS+=$errors;
   chdir($BUILD_DIR);
}

################################################################################

sub convert_binary {

   if($TOTAL_ERRORS > 0) {
      return;
   }

   chdir("$BUILD_DIR\\$OBJ_DIR");

   if(cpu($CPU_AVR8515)) {
      print("CONVERTING...\n");

      #$errors+=run($GNU_PREFIX."objcopy.exe -O avrobj -R .eeprom $TARGET.elf $TARGET.obj");
      #$errors+=run($GNU_PREFIX."objcopy.exe -O ihex -R .eeprom $TARGET.elf $TARGET.rom");
      #$errors+=run("elfcoff $TARGET.elf -coff $TARGET.cof $TARGET.sym");
      #copy_file("coff\\$TARGET.cof","$TARGET.cof");
      #copy_file("coff\\$TARGET.sym","$TARGET.sym");
      #copy_file("coff\\$TARGET.S","$TARGET.S");
      #$errors+=run($GNU_PREFIX."objcopy.exe -j .eeprom --set-section-flags=.eeprom=\"alloc,load\" --change-section-lma .eeprom=0 -O ihex $TARGET.elf $TARGET.eep");

      #$errors+=run($GNU_PREFIX."objcopy.exe "
      #   ."--debugging "
      #   ."-O coff-ext-avr "
      #   ."--change-section-address .data-0x800000 "
      #   ."--change-section-address .bss-0x800000 "
      #   ."--change-section-address .noinit-0x800000 "
      #   ."--change-section-address .eeprom-0x810000 "
      #   ."$TARGET.elf $TARGET.cof");

      $errors+=run($GNU_PREFIX."objcopy.exe "
         ."--input-target=elf32-avr "
         ."--output-target=ihex "
         ."-j .vectors "
         ."-j .text "
         ."--verbose "
         ."$TARGET.elf $TARGET.hex");

      print("OK\n");
   } elsif(cpu($CPU_M68HC11)) {
      print("CONVERTING...\n");
      $errors+=run($GNU_PREFIX."objcopy.exe "
         ."--output-target=srec "
         #."--srec-forceS3 "
         ."--strip-all "
         ."--strip-debug "
         ."--verbose "
         ."$TARGET.elf $TARGET.s19");
      print("OK\n");
   }
   print("\n");

   $TOTAL_ERRORS+=$errors;
   chdir($BUILD_DIR);
}

################################################################################

sub show_target_file {

   if($TOTAL_ERRORS > 0) {
      return;
   }

   my $tempfile="$TARGET.dir";
   my $target_file=""
      .ifcpu($CPU_AVR8515,"$OBJ_DIR\\$TARGET.elf")
      .ifcpu($CPU_M68HC11,"$OBJ_DIR\\$TARGET.S19")
      ;
   run("dir $target_file > $tempfile");

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
      .ifcpu($CPU_AVR8515,"--architecture=avr:2 ")
      .ifcpu($CPU_M68HC11,"--architecture=m68hc11 ")
      #."--section=.text "
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

   if($DEBUG) { dump_file($tempfile); }
   debug("");

   open(TEMP,"<$tempfile");
   my $line;
   while($line=<TEMP>) {
      if(index($line,"2**")>-1) {
         # combine with next line
         $line=$line.<TEMP>;
         $line=~s/\012//g;
         $line=~s/\015//g;
      }

      if(index($line,"2**")>-1) {
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
   } elsif($header eq "vectors") {    return ( "ROM"    , "vectors (init)" );
   } elsif($header eq "specvect") {   return ( "ROM"    , "vectors (init)" );
   } elsif($header eq "data") {       return ( "ROM,RAM", "data (init)"    );
   } elsif($header eq "normvect") {   return (     "RAM", "vectors (zero)" );
   } elsif($header eq "bss") {        return (     "RAM", "data (zero)"    );
   } elsif($header eq "common") {     return (     "RAM", "data (uninit)"  );
   } elsif($header eq "stack") {      return (     "RAM", "stack (zero)"   );
   } elsif($header eq "heap") {       return (     "RAM", "heap (zero)"    );
   } elsif($header eq "end_of_rom") { return ( ""       , "???"            );
   } elsif($header eq "softregs") {   return ( ""       , "page0"          );
   } elsif($header eq "comment") {    return ( ""       , "debug"          );
   } elsif($header eq "debug") {      return ( ""       , "debug"          );
   } elsif($header eq "stab") {       return ( ""       , "debug"          );
   } elsif($header eq "stabstr") {    return ( ""       , "debug"          );
   } else {                           return ( ""       , "???"            );
   }
}

################################################################################
#   D E T E R M I N I N G   D E P E N D E N C I E S
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
      # look for "#include"
      # ignore standard includes (in angle brackets)
      if((index($line,"#include")>-1)&&(index($line,"<")==-1)) {
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
#   F U N C T I O N S
################################################################################

sub cpu {
   my ($cpu)=@_;
   if($CPU==$cpu) { return 1; }
   else { return 0; }
}

################################################################################

sub ifcpu {
   my ($cpu,$str)=@_;
   if($CPU==$cpu) { return $str; }
   else { return ""; }
}

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
      print("MOVING [$src] -> [$dest]\n");
   }
   move($src,$dest);
}

################################################################################

sub copy_file {
   my $src=$_[0];
   my $dest=$_[1];
   if($DEBUG) {
      print("COPYING [$src] => [$dest]\n");
   }
   copy($src,$dest);
}

################################################################################

sub dump_file {
   my $file=$_[0];
   open(DUMP,"<$file");
   my $line;
   while($line=<DUMP>) {
      $line=~s/\012//g;
      $line=~s/\015//g;
      print("$line\n");
   }
   close(DUMP);
}

################################################################################

sub make_directory {
   my $dir=$_[0];
   my $parent=directory_of($dir);
   if((length($parent)>0)&&($parent ne "\\")&&($parent ne ".")) {
      if(! -e $parent) {
         make_directory($parent);
      }
   }
   if($DEBUG) {
      print("MAKING DIRECTORY [$dir]\n");
   }
   mkdir($dir,0777);
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
   # remove trailing backslash
   $dir=~s/\\$//g;
   # check for root directory
   if($dir eq "\\") { return "\\"; }
   # check for current directory
   if(index($dir,"\\")==-1) { return "."; }
   # remove stuff after last backslash
   $dir=~s/\\[^\\]*$//g;
   # check for root directory (again)
   if(length($dir)==0) { return "\\"; }
   # done
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


