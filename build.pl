
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
show_results();
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
               "app\\test.c",
               "kernel\\kernel.c",
               "kernel\\system.c",
               "kernel\\crt11.s",
               "kernel\\clock.c",
               "kernel\\error.c",
               "kernel\\intvect.c",
   );

   print("OK\n");
}

################################################################################

sub setup_compiler {
   print("setting up compiler...");
   $compiler_home="c:\\programs\\hc11\\gcc";
   add_to_path("$compiler_home\\BIN");
   print("OK\n");
}

################################################################################

sub setup_linker {
   print("setting up linker...");
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
      if( ! -e "$obj_path" ) {
         $work=1;
      } else {
         $filetime= -M $src_path;
         $objtime= -M $obj_path;
         if( $filetime < $objtime ) {
            $work=1;
         }
      }

      # IF WE HAVE WORK TO DO, THEN DO IT

      if( $work == 1 ) {
         if($src_ext eq "c") {
            print($indent."COMPILING...");
            if($debug) { print("\n"); }
            chdir($src_dir);
            $errors+=run("gcc.exe "
                        ."-m68hc11 "
                        ."-DGCC "
                        ."-I$build_dir -I. "
                        ."-mshort "
                        ."-O "  # was ."O0 "  # oh-zero
                        ."-fomit-frame-pointer "
                        ."-msoft-reg-count=0 "
                        ."-c "
                        ."-g "
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
            if($debug) { print("\n"); }
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
      if($errors > 0) {
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

   if($total_errors > 0) {
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
      ."--trace "
      ."-nostdlib "
      ."-nostartfiles "
      ."-defsym init_sp=0xB5FF "
      ."-defsym _.tmp=0x0 "
      ."-defsym _.z=0x2 "
      ."-defsym _.xy=0x4 "
      ."-Map $TARGET.map --cref "
      ."--oformat=elf32-m68hc11 "
      ."-o $TARGET.elf "
      ."$ofiles");
   print("OK\n");
   print("\n");


   if($errors==0) {
      print("CONVERTING...\n");
      $errors+=run("objcopy.exe "
         ."--output-target=srec "
         ."--strip-all "
         ."--strip-debug "
         ."$TARGET.elf $TARGET.s19");
      print("OK\n");
      print("\n");
   }


   if($errors==0) {
      print("GENERATING LISTING...\n");
      $errors+=run("objdump.exe "
         ."--disassemble-all "
         ."--architecture=m68hc11 "
         ."--section=.text "
         #."--debugging "
         ."$TARGET.elf > $TARGET.lst");
      print("OK\n");
      print("\n");
   }

   $total_errors+=$errors;
   chdir($build_dir);
}

################################################################################

sub show_results {
   if($total_errors > 0) {
      return;
   }
   print("done\n");
   print("--- SHOW THE RESULTS ---\n");
   run("dir $OBJ_DIR\\$TARGET.s19");
   print("\n");
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
   if($debug) {
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
   while(($changes*2)<$#args) {
      $search[$changes]=@args[$changes*2];
      $replace[$changes]=@args[$changes*2+1];
      $changes++;
   }

   if($debug) {
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
      if(($debug>0)&&($replacements_made>0)) {
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
   if($debug) {
      print("RUNNING [$cmd]\n");
   }
   $rc=system($cmd);
   return $rc;
}

################################################################################

sub move_file {
   my $src=$_[0];
   my $dest=$_[1];
   if($debug) {
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

