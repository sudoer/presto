
################################################################################
#   B A S I C   I N C L U D E S   A N D   D E B U G   I N F O
################################################################################

# fast move/copy routines
use File::Copy;

# turn debug info on/off
$debug=0;

setup_project();
setup_compiler();
setup_linker();
prepare();
compile_stage();
link_stage();
show_target_file();
generate_listing();
show_memory_usage();
cleanup();
exit;

################################################################################

sub setup_project {
   print("--- CONFIGURATION ---\n");

   # SETTING UP THE PROJECT

   print("setting up project...");
   $OBJ_DIR="obj";
   $TARGET="presto";
   @SRC_FILES=(
               "presto\\cpu\\crt11.s",
               "presto\\cpu\\boot.c",
               "presto\\cpu\\error.c",
               "presto\\cpu\\intvect.c",
               "presto\\cpu\\hwtimer.c",
               "presto\\kernel\\clock.c",
               "presto\\kernel\\kernel.c",
               "presto\\kernel\\mail.c",
               "presto\\kernel\\timer.c",
               "presto\\kernel\\semaphore.c",
               "presto\\kernel\\memory.c",
               "app\\main.c",
               "app\\debugger.c",
               "services\\serial.c",
               "services\\string.c",
   );

   print("OK\n");
}

################################################################################

sub setup_compiler {
   print("setting up compiler...");
   $compiler_home="c:\\programs\\hc11\\gnu";
   add_to_path("$compiler_home\\bin");
   print("OK\n");
}

################################################################################

sub setup_linker {
   print("setting up linker...");
   $lib_dir=$compiler_home."\\lib";
   print("OK\n");
}

################################################################################

sub prepare {

   print("--- PREPARING ---\n");

   # CLEANING UP OLD FILES

   print("deleting old target file...");
   unlink("$OBJ_DIR\\$TARGET.s19");
   print("OK\n");

   # CREATING OBJECT DIRECTORY

   print("creating object directory...");
   mkdir($OBJ_DIR,0777);
   print("OK\n");

   # REMEMBER WHERE YOU STARTED

   chop($build_dir=`cd`);
   $build_dir=~s/^[A-Za-z]://g;  # remove C:

   print("\n");
}

################################################################################

sub compile_stage {

   print("--- COMPILING AND ASSEMBLING ---\n");

   $indent="   ";
   @OBJS=();
   $total_errors=0;

   # LOOPING THROUGH OBJECT FILES

   foreach $src_path (@SRC_FILES) {
      print("FILE $src_path\n");
      $errors=0;

      # DETERMINE FILE NAMES, BASE NAMES, EXTENSIONS, PATHS

      $src_dir=directory_of($src_path);
      $src_name=basename($src_path);
      $src_base=chop_extension($src_name);
      $src_ext=extension_of($src_path);
      debug("src_name=[$src_name]");
      debug("src_path=[$src_path]");
      debug("src_dir=[$src_dir]");
      debug("src_base=[$src_base]");
      debug("src_ext=[$src_ext]");

      $obj_name="$src_base.o";

      $obj_path="$OBJ_DIR\\$obj_name";
      debug("obj_name=[$obj_name]");
      debug("obj_path=[$obj_path]");

      # DETERMINE IF WE NEED TO DO ANYTHING FOR THIS FILE

      $work=0;
      if ( ! -e "$obj_path" ) {
         $work=1;
      } else {
         $filetime= -M $src_path;
         $objtime= -M $obj_path;
         if ( $filetime < $objtime ) {
            $work=1;
         }
      }

      # IF WE HAVE WORK TO DO, THEN DO IT

      if ( $work == 1 ) {
         if ($src_ext eq "c") {
            print($indent."COMPILING...");
            if ($debug) { print("\n"); }
            chdir($src_dir);
            $errors+=run("gcc.exe "
                        ."-m68hc11 "
                        ."-DGCC "
                        ."-I$build_dir -I. -I$build_dir\\presto "
                        ."-mshort "
                        ."-O "  # was ."O0 "  # oh-zero
                        ."-fomit-frame-pointer "
                        ."-msoft-reg-count=0 "
                        ."-funsigned-char "
                        ."-c "
                        ."-g "
                        ."-Wall "
                        #."-Werror "
                        ."-Wa,-L,-ahlns=$build_dir\\$OBJ_DIR\\$src_base.lst "
                        ."-o $build_dir\\$OBJ_DIR\\$src_base.o "
                        ."$src_name");
            print("OK\n");
            #print($indent."GENERATING LISTINGS...");
            #if($debug) { print("\n"); }
            #chdir("$build_dir\\$OBJ_DIR");
            #$errors+=run("objdump.exe --source $src_base.o > $src_base.lst ");
            #print("OK\n");
            chdir($build_dir);
         } elsif($src_ext eq "s") {
            print($indent."ASSEMBLING...");
            if ($debug) { print("\n"); }
            chdir($src_dir);
            $errors+=run("as.exe "
                        ."-a -L -ahlns=$build_dir\\$OBJ_DIR\\$src_base.lst "
                        ."-o $build_dir\\$OBJ_DIR\\$src_base.o "
                        ."$src_name");
            print("OK\n");
            chdir($build_dir);
         } else {
            print("HUH?\n");
         }
      } else {
         print("OK\n");
      }

      print("\n");

      # RECORD THE FILES THAT WE NEED TO LINK TOGETHER

      push(@OBJS,$obj_name);

      $total_errors+=$errors;
      if ($errors > 0) {
         print("\n");
         print("ERRORS IN COMPILE ... stopping\n");
         return;
      }

   }

   # END OF --- COMPILING AND ASSEMBLING ---

   print("\n");

}

################################################################################

sub link_stage {

   if ($total_errors > 0) {
      print("errors compiling, skipping the link stage\n");
      print("\n");
      return;
   }

   chdir("$build_dir\\$OBJ_DIR");

   $ofiles="";
   foreach $obj_file (@OBJS) {
      $ofiles=$ofiles." ".$obj_file;
   }


   print("LINKING...\n");
   $errors+=run("ld.exe "
      ."--script ../memory.x "
      ."-m m68hc11elf "
      ."--trace "
      ."-nostdlib "
      ."-nostartfiles "
      ."-defsym _.tmp=0x0 "
      ."-defsym _.z=0x2 "
      ."-defsym _.xy=0x4 "
      ."-Map $TARGET.map --cref "
      ."--oformat=elf32-m68hc11 "
      ."-o $TARGET.elf "
      ."$ofiles "
      ."-L$lib_dir -lgcc");
   print("OK\n");
   print("\n");


   if ($errors==0) {
      print("CONVERTING...\n");
      $errors+=run("objcopy.exe "
         ."--output-target=srec "
         ."--strip-all "
         ."--strip-debug "
         ."$TARGET.elf $TARGET.s19");
      print("OK\n");
      print("\n");
   }


   $total_errors+=$errors;
   chdir($build_dir);
}

################################################################################

sub show_target_file {

   if ($total_errors > 0) {
      return;
   }
   print("--- TARGET IS READY -- SHOW THE RESULTS ---\n");
   run("dir $OBJ_DIR\\$TARGET.s19");
   print("\n");
}

################################################################################

sub generate_listing {

   if ($total_errors > 0) {
      return;
   }

   chdir("$build_dir\\$OBJ_DIR");

   print("GENERATING LISTING...\n");
   $errors+=run("objdump.exe "
      ."--disassemble-all "
      ."--architecture=m68hc11 "
      ."--section=.text "
      #."--debugging "
      ."$TARGET.elf > $TARGET.lst");
   print("OK\n");
   print("\n");

   $total_errors+=$errors;
   chdir($build_dir);
}

################################################################################

sub show_memory_usage {

   if ($total_errors > 0) {
      return;
   }

   my $tempfile="headers.txt";  #$TARGET.".tmp";
   run("objdump.exe -h $OBJ_DIR\\$TARGET.elf > $tempfile");

   my %memusage_sectsize;
   my @memusage_ramsections;
   my @memusage_romsections;
   my %memusage_used;

   open(TEMP,"<$tempfile");
   my $line;
   while ($line=<TEMP>) {
      if (index($line,"2**0")>-1) {
         # combine with next line
         $line=$line.<TEMP>;
         $line=~s/\012//g;
         $line=~s/\015//g;
      }

      if (index($line,"2**0")>-1) {
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



   #print("SECTIONS\n");
   #print("------------\n");
   #foreach $section (sort keys %memusage_sectsize) {
   #   printf("%s %d\n",$section,$memusage_sectsize{$section});
   #}
   #print("\n");



   my $rom_space=0;
   my $ram_space=0;
   print("MEMORY USAGE\n");
   print("------------\n");
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

sub cleanup {
   print("--- CLEAN UP ---\n");
   print("removing intermediate files...");
   #unlink <*.s>;
   #unlink <*.i>;
   print("OK\n");

}

################################################################################
#   F U N C T I O N S
################################################################################

sub start_action {
   my $string=$_[0];
   print("$string...");
}

################################################################################

sub end_action {
   my $string=$_[0];
   print("$string\n");
}

################################################################################

sub debug {
   my $string=$_[0];
   if ($debug) {
      print("   $string\n");
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
   while (($changes*2)<$#args) {
      $search[$changes]=@args[$changes*2];
      $replace[$changes]=@args[$changes*2+1];
      $changes++;
   }

   if ($debug) {
      for $count (0..$changes-1) {
         print("replace [$search[$count]] with [$replace[$count]]\n");
      }
   }

   open(OLD,$filename);
   open(NEW,">$tempfile");
   while ($line=<OLD>) {
      $replacements_made=0;
      for $count (0..$changes-1) {
         while ($find=index($line,$search[$count])>-1) {
            substr($line,$find,length($search[$count]))=$replace[$count];
            $replacements_made++;
         }
      }
      if (($debug>0)&&($replacements_made>0)) {
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
   if ($debug) {
      print("RUNNING [$cmd]\n");
   }
   $rc=system($cmd);
   return $rc;
}

################################################################################

sub move_file {
   my $src=$_[0];
   my $dest=$_[1];
   if ($debug) {
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
   if (index($dir,$path)<0) {
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


