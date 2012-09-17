
################################################################################
#   B A S I C   I N C L U D E S   A N D   D E B U G   I N F O
################################################################################

# fast move/copy routines
use File::Copy;
use Cwd;

heading("PREPARING");
prepare();
parse_command_line();

heading("CONFIGURATION");
determine_cpu();
cpu_specific_settings();

heading("PRE-BUILD CLEANING");
prebuild_cleaning();

heading("COMPILING AND ASSEMBLING");
compile_stage();

heading("LINKING");
link_stage();

heading("GENERATING DEBUG INFORMATION");
generate_listing();
show_memory_usage();

heading("PREPARE THE LOAD FILE");
convert_binary();

exit;

################################################################################
#   C U S T O M I Z E   T H I S   P A R T
################################################################################

sub parse_command_line {

   # turn debug info on/off
   print("parsing command-line arguments...");
   $DEBUG=0;
   foreach $arg (@ARGV) {
      if(tolower($arg) eq "debug") {
         $DEBUG=1;
      } elsif(tolower($arg) eq "batch") {
         $BATCH=1;
      }	else {
          $project_file=$arg;
      }
   }
   print("OK\n");

   if($BATCH) {
      open(BATCH,">build.bat");
   }

   if(!open(PRJ,"<$project_file")) {
      print("\n");
      print("usage: PERL BUILD.PL <project file> [DEBUG]\n");
      exit(1);
   }

   # defaults

   @SRC_FILES=();
   @INCLUDE_DIRS=("$BUILD_DIR",".");

   $section="";
   while($line=<PRJ>) {
      # do not chop($line); messes up on last line... instead s/\012//g;
      $line=~s/\012//g;
      $line=~s/\015//g;
      $line=~s/#.*$//g;
      $line=~s/^ *//g;
      $line=~s/ *$//g;

      if(length($line)==0) {

         # line is blank, or was a comment

      } elsif(index($line,"[")==0) {

         # line is a new section

         $section=toupper($line);

         # trim off brackets
         $section=~s/^\[//g;
         $section=~s/\]$//g;
      } else {

         # line has some information

         if($section eq "OPTIONS") {
            ($option,$value)=split('=',$line);
            debug("OPTION $option = $value");
            if($option eq "CPU") {              $OPT_CPU=$value;
            } elsif($option eq "TARGET") {      $OPT_TARGET=$value;
            } elsif($option eq "OBJ_DIR") {     $OPT_OBJDIR=$value;
            } elsif($option eq "MEM_MAP") {     $OPT_MEMMAP=$value;
            } else {  print("unknown option '$option'\n");
            }
         } elsif($section eq "SOURCE") {
            $line=unix_slashes($line);
            debug("SOURCE $line");
            push(@SRC_FILES,$line);
         } elsif($section eq "INCLUDE") {
            $line=unix_slashes($line);
            debug("INCLUDE $line");
            push(@INCLUDE_DIRS,"$BUILD_DIR/".$line);
         } else {
            print("unknown section '$section'\n");
         }
      }
   }
   close(PRJ);

   print("OK\n");
}

################################################################################

sub determine_cpu {

   # DEFINE CPU ARCHITECTURES
   print("defining architectures...");
   $CPU_M68HC11=1;
   $CPU_AVR8515=2;
   $CPU_MEGA169=3;
   print("OK\n");

   $CPU=0;
   if(tolower($OPT_CPU) eq "m68hc11")  { $CPU=$CPU_M68HC11;   }
   if(tolower($OPT_CPU) eq "avr8515")  { $CPU=$CPU_AVR8515;   }
   if(tolower($OPT_CPU) eq "mega169")  { $CPU=$CPU_MEGA169; }

   if($CPU==0) {
      print("ERROR - unknown CPU type ($OPT_CPU)\n");
   } else {
      print("CPU is $OPT_CPU\n");
   }

}

################################################################################

sub cpu_specific_settings {

   print("setting up compiler...");
   $COMPILER_HOME=""
      .ifcpu($CPU_M68HC11,"c:/programs/embedded/hc11/gnu")
      .ifcpu($CPU_AVR8515,"c:/programs/embedded/avr/gnu")
      .ifcpu($CPU_MEGA169,"c:/programs/embedded/avr/gnu")
      ;
   add_to_path("$COMPILER_HOME/bin");
   $CPU_COMPILER_OPTIONS=""
       # TELL MY PROGRAMS WHAT CPU WE'RE USING
       .ifcpu($CPU_M68HC11,"-DCPU=M68HC11 ")      # 
       .ifcpu($CPU_AVR8515,"-DCPU=AVR8515 ")      # 
       .ifcpu($CPU_MEGA169,"-DCPU=MEGA169 ")      # 
       # DEFINE ARCHITECTURE
       .ifcpu($CPU_M68HC11,"-m68hc11 ")           # platform, hc11 or hc12
       .ifcpu($CPU_AVR8515,"-mmcu=at90s8515 ")    # platform, which AVR
       .ifcpu($CPU_MEGA169,"-mmcu=atmega169 ")    # platform, which AVR
       # WHERE TO FIND INCLUDE FILES
       .$include_path                             # include path
       # OPTIMIZATION
       .ifcpu($CPU_M68HC11,"-Os ")                # optimization, was (oh-zero)
       .ifcpu($CPU_AVR8515,"-O1 ")                # optimization level
       .ifcpu($CPU_MEGA169,"-O1 ")                # optimization level
       # STACK FRAMES
       ."-fomit-frame-pointer "                   # when possible do not generate stack frames
       # SIZES OF TYPES
       ."-funsigned-char "                        # chars are unsigned
       # M68HC11-SPECIFIC OPTIONS
       .ifcpu($CPU_M68HC11,"-mshort ")            # use short ints
       .ifcpu($CPU_M68HC11,"-msoft-reg-count=0 ") # soft registers available
       # STORAGE
       ."-fwritable-strings "                     # store strings in "strings" section, not inline
       ;
   print("OK\n");

   ###

   print("setting up linker...");
   $GCC_LIB=$COMPILER_HOME
      .ifcpu($CPU_M68HC11,"/lib/gcc-lib/m6811-elf/3.0.4/libgcc.a")
      .ifcpu($CPU_AVR8515,"/lib/gcc-lib/avr/3.3.1/libgcc.a")
      .ifcpu($CPU_MEGA169,"/lib/gcc-lib/avr/3.3.1/libgcc.a")
      ;
   $CPU_RUNTIME_LIB=""
      .ifcpu($CPU_AVR8515,"$COMPILER_HOME/avr/lib/crts8515.o ")
      .ifcpu($CPU_MEGA169,"$COMPILER_HOME/avr/lib/avr5/crtm169.o ")
      ;
   $CPU_LINKER_OPTIONS=""
      # DEFINE ARCHITECTURE
      .ifcpu($CPU_M68HC11,"-m m68hc11elf ")            # architecture, file format
      .ifcpu($CPU_AVR8515,"-m avr85xx ")               # architecture, file format
      .ifcpu($CPU_MEGA169,"-m avr5 ")                  # architecture, file format
      # M68HC11-SPECIFIC STUFF
      .ifcpu($CPU_M68HC11,"-nostdlib ")                # do not include C standard library
      .ifcpu($CPU_M68HC11,"-nostartfiles ")            # do not include crt0.s
      .ifcpu($CPU_M68HC11,"-defsym _.tmp=0x0 ")        # temporary "soft" register
      .ifcpu($CPU_M68HC11,"-defsym _.z=0x2 ")          # temporary "soft" register
      .ifcpu($CPU_M68HC11,"-defsym _.xy=0x4 ")         # temporary "soft" register
      .ifcpu($CPU_M68HC11,"--oformat=elf32-m68hc11 ")  # output file format
      # AVR-SPECIFIC STUFF
      .ifcpu($CPU_AVR8515,"-nostdlib ")                # do not include C standard library
      .ifcpu($CPU_MEGA169,"-nostdlib ")                # do not include C standard library
      ;
   print("OK\n");

   ###

   print("setting up GNU tools...");
   $GNU_PREFIX=""
      .ifcpu($CPU_M68HC11,"m6811-elf-")
      .ifcpu($CPU_AVR8515,"avr-")
      .ifcpu($CPU_MEGA169,"avr-")
      ;
   $CPU_ARCHITECTURE=""
      .ifcpu($CPU_M68HC11,"m68hc11 ")
      .ifcpu($CPU_AVR8515,"avr:2 ")
      .ifcpu($CPU_MEGA169,"avr:5 ")
      ;
   print("OK\n");

   ###

   print("setting up loader-specific options...");
   $CPU_CONVERSION_OPTIONS=""
      .ifcpu($CPU_M68HC11,"--output-target=srec --strip-all --strip-debug ")
      .ifcpu($CPU_AVR8515,"--input-target=elf32-avr --output-target=ihex -j .vectors -j .text ")
      .ifcpu($CPU_MEGA169,"--input-target=elf32-avr --output-target=ihex -j .vectors -j .text ")
      ;
   $LOAD_EXT=""
      .ifcpu($CPU_M68HC11,"S19")
      .ifcpu($CPU_AVR8515,"HEX")
      .ifcpu($CPU_MEGA169,"HEX")
      ;
   print("OK\n");

   ###

   print("setting up endian...");
   $CPU_ENDIAN=""
      .ifcpu($CPU_M68HC11,"BIG_ENDIAN ")
      .ifcpu($CPU_AVR8515,"LITTLE_ENDIAN ")
      .ifcpu($CPU_MEGA169,"LITTLE_ENDIAN ")
      ;
   print("OK\n");



#endif // BIG_ENDIAN

#ifdef LITTLE_ENDIAN

}

################################################################################
#   T H I S   S T U F F   S H O U L D   S T A Y   T H E   S A M E
################################################################################

sub prepare {

   # flush after print();
   $|=1;

   # REMEMBER WHERE YOU STARTED
   print("remembering build directory...");
   $BUILD_DIR=cwd();  # chop($BUILD_DIR=`cd`); was SLOW!
   $BUILD_DIR=~s/^[A-Za-z]://g;  # remove C:
   #$BUILD_DIR=dos_slashes($BUILD_DIR);
   print("OK\n");

}

################################################################################

sub prebuild_cleaning {

   # CREATE OBJECT DIRECTORY
   print("creating object directory...");
   debug("");
   make_directory($OPT_OBJDIR);
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

      $src_path="$BUILD_DIR/$src_file";
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
      $obj_path="$BUILD_DIR/$OPT_OBJDIR/$obj_name";
      #debug("obj_name=[$obj_name]");
      #debug("obj_path=[$obj_path]");
      $lst_path="$BUILD_DIR/$OPT_OBJDIR/$src_base.lst";

      # DETERMINE IF WE NEED TO DO ANYTHING FOR THIS FILE

      change_directory($src_dir);

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
            $errors+=run($GNU_PREFIX."gcc.exe "

               # CPU-specific options
               ."$CPU_COMPILER_OPTIONS "
               ."-D $CPU_ENDIAN "

               # INCLUDES
               ."$include_path "

               # WARNINGS AND ERRORS
               ."-Wall "                         # enable all warnings
               #."-Werror "                      # treat warnings as errors

               # LISTINGS
               ."-Wa,-L,-ahlns=$lst_path "       # inc local sym, generate list file

               # WHAT TO DO
               ."-c "                            # compile only, do not link
               ."-g "                            # include debug info
               ."-o $obj_path "                  # object file (output)
               ."$src_name"                      # source file (input)
            );
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

      change_directory($BUILD_DIR);
   }

}

################################################################################

sub link_stage {

   if($TOTAL_ERRORS > 0) {
      print("errors compiling, skipping the link stage\n");
      print("\n");
      return;
   }

   change_directory("$BUILD_DIR/$OPT_OBJDIR");

   $ofiles="";
   foreach $obj_file (@OBJS) {
      $ofiles=$ofiles." ".$obj_file;
   }

   my $trace="";
   if($DEBUG) { $trace="--trace"; }

   print("LINKING...\n");


   $errors+=run($GNU_PREFIX."ld.exe "

      # CPU-specific options
      ."$CPU_LINKER_OPTIONS "

      # DEFINE MEMORY MAP
      ."--script $BUILD_DIR/$OPT_MEMMAP "

      # GIVE FEEDBACK ON THE SCREEN, IN MAP FILE
      ."$trace "                                       # print file names as they are completed
      ."-Map $OPT_TARGET.map --cref "                      # create memory map file

      # OUTPUT FILE
      ."-o $OPT_TARGET.elf "

      # INPUT FILES
      ."$ofiles "
      ."$CPU_RUNTIME_LIB "
      ."$GCC_LIB "
   );

   print("OK\n");
   print("\n");

   $TOTAL_ERRORS+=$errors;
   change_directory($BUILD_DIR);
}

################################################################################

sub convert_binary {

   if($TOTAL_ERRORS > 0) {
      return;
   }

   change_directory("$BUILD_DIR/$OPT_OBJDIR");

   print("CONVERTING...\n");
   $errors+=run($GNU_PREFIX."objcopy.exe "
      ."$CPU_CONVERSION_OPTIONS "
      ."--verbose "
      ."$OPT_TARGET.elf "
      ."$OPT_TARGET.$LOAD_EXT");
   print("OK\n");
   print("\n");

   $TOTAL_ERRORS+=$errors;
   change_directory($BUILD_DIR);
}

################################################################################

# sub show_target_file {
# 
#    if($TOTAL_ERRORS > 0) {
#       return;
#    }
# 
#    my $tempfile="$OPT_TARGET.dir";
#    my $target_file="$OPT_OBJDIR\\$OPT_TARGET.$LOAD_EXT";
#    $target_file=dos_slashes($target_file);
#    run("dir $target_file > $tempfile");
# 
#    open(TEMP,"<$tempfile");
#    my $line;
#    while($line=<TEMP>) {
#       if(index(tolower($line),tolower(basename($OPT_TARGET.$LOAD_EXT)))>-1) {
#          print($line);
#       }
#    }
#    close(TEMP);
#    delete_file($tempfile);
#    print("\n");
# }

################################################################################

sub generate_listing {

   if($TOTAL_ERRORS > 0) {
      return;
   }

   change_directory("$BUILD_DIR/$OPT_OBJDIR");

   print("GENERATING LISTING...\n");
   $errors+=run($GNU_PREFIX."objdump.exe "
      ."--disassemble-all "
      ."--architecture=$CPU_ARCHITECTURE "
      #."--section=.text "
      #."--debugging "
      ."$OPT_TARGET.elf > $OPT_TARGET.lst");
   print("OK\n");
   print("\n");

   $TOTAL_ERRORS+=$errors;
   change_directory($BUILD_DIR);
}

################################################################################

sub show_memory_usage {

   if($TOTAL_ERRORS > 0) {
      return;
   }

   print("MEMORY USAGE\n");
   print("------------\n");

   my $tempfile="headers.txt";  #$OPT_TARGET.".tmp";
   run($GNU_PREFIX."objdump.exe -h $OPT_OBJDIR/$OPT_TARGET.elf > $tempfile");

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
   delete_file($tempfile);

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
   } elsif($header eq "string") {     return ( "ROM"    , "strings"        );
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
         # path may be absolute
         if(-f $givenfile) {
            debug(" H file (".$givenfile.") has an absolute path");
            $fullpathname=$givenfile;
         } else {
            debug("ERROR! H file (".$givenfile.") not found");
            return 0;
         }
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
      #$line=dos_slashes($line);
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
      $dir=unix_slashes($dir);
      if(-f "$dir/$h_file") {
         #debug("INCLUDE FILE [$h_file] is [$dir/$h_file]");
         return "$dir/$h_file";
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
   delete_file($filename);
   move($tempfile,$filename);
}

################################################################################

sub dos_slashes {
   my $string=$_[0];
   $string=~s,\/+,\\,g;       #  /  ->  \
   return $string;
}

################################################################################

sub unix_slashes {
   my $string=$_[0];
   $string=~s,\\+,\/,g;     #  \  ->  /
   return $string;
}

################################################################################

sub run {
   my $cmd=$_[0];
   my $rc;
   $cmd=dos_slashes($cmd);
   if($DEBUG) {
      print("RUNNING [$cmd]\n\n");
   }
   if($BATCH) {
      print(BATCH "$cmd\n");
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
   if($BATCH) {
      print(BATCH "move $src $dest\n");
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
   if($BATCH) {
      print(BATCH "copy $src $dest\n");
   }
   copy($src,$dest);
}

################################################################################

sub delete_file {
   my $filename=$_[0];
   unlink($filename);
   if($BATCH) {
      $filename=dos_slashes($filename);
      print(BATCH "del $filename\n");
   }
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
   if((length($parent)>0)&&($parent ne "/")&&($parent ne ".")) {
      if(! -e $parent) {
         make_directory($parent);
      }
   }
   if($DEBUG) {
      print("MAKING DIRECTORY [$dir]\n");
   }
   if($BATCH) {
      $dir=dos_slashes($dir);
      print(BATCH "mkdir $dir\n");
   }
   mkdir($dir,0777);
}

################################################################################

sub change_directory {
   my $dir=$_[0];
   chdir($dir);
   if($BATCH) {
      $dir=dos_slashes($dir);
      print(BATCH "cd $dir\n");
   }
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
   if($BATCH) {
      print(BATCH "SET $varname=$value\n");
   }
}

################################################################################

sub add_to_path {
   my $dir=tolower($_[0]);
   $dir=dos_slashes($dir);
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
   # force all slashes to unix style
   $dir=unix_slashes($dir);
   # remove trailing slashes
   $dir=~s/\/+$//g;
   # check for root directory
   if($dir eq "/") { return "/"; }
   # check for current directory
   if(index($dir,"/")==-1) { return "."; }
   # remove stuff after last slash
   $dir=~s/\/[^\/]*$//g;
   # check for root directory (again)
   if(length($dir)==0) { return "/"; }
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
   # get rid of everything before slashes
   $filename=~s/^.*\///g;
   return $filename;
}

################################################################################

sub dec2hex {
   return sprintf("%x",$_[0]);
}

################################################################################


