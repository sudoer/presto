
################################################################################
#   B A S I C   I N C L U D E S   A N D   D E B U G   I N F O
################################################################################

# turn debug info on/off
$debug=0;

# fast move/copy routines
use File::Copy;

################################################################################
#   P R O J E C T   C O N F I G U R A T I O N
################################################################################

print("--- CONFIGURATION ---\n");

# SETTING UP THE PROJECT

print("setting up project...");
$OBJ_DIR="obj";
$TARGET="hbtest";
@SRC_FILES=("kernel\\kernel.c",
#           "kernel\\task_sw.asm",
            "kernel\\clock.c",
            "kernel\\system.c",
            "kernel\\intvect.c",
#           "kernel\\crt0.asm",
            "services\\debugger.c",
            "services\\lcd.c",
            "services\\i2c.c",
            "services\\inputs.c",
            "services\\motors.c",
            "services\\serial.c",
            "services\\sound.c",
            "utils\\string.c",
            "app\\test.c");
@LIBS=();
print("OK\n");

################################################################################
#   T O O L   C O N F I G U R A T I O N
################################################################################

# COMPILER CONFIGURATION

print("setting up compiler...");
$compiler_home="c:\\programs\\hc11\\icc";
add_to_path("$compiler_home\\BIN");
setenv("ICC11_INCLUDE","$compiler_home\\INCLUDE");
setenv("DOS4G","quiet");
setenv("DOS16M","0");
print("OK\n");

print("setting up linker...");
$icc11_lib="$compiler_home\\LIB";
setenv("ICC11_LIB",$icc11_lib);
print("OK\n");

# END OF --- CONFIGURATION ---

print("\n");

################################################################################
#   P R E P A R I N G
################################################################################

print("--- PREPARING ---\n");

# CLEANING UP OLD FILES

print("deleting old intermediate files...");
unlink <*.bak>;
unlink <*.aaa>;
unlink <*.s>;
unlink <*.i>;
#rem del /s/y/x/q %OBJ_DIR% >&> NUL:
#rem del /s/q *.s19 >&> NUL:
print("OK\n");

# CREATING OBJECT DIRECTORY

print("creating object directory...");
mkdir($OBJ_DIR,0777);
print("OK\n");

# REMEMBER WHERE YOU STARTED

chop($base_dir=`cd`);
$base_dir=~s/^[A-Za-z]://g;  # remove C:

# END OF --- PREPARING ---

print("\n");

################################################################################
#   C O M P I L I N G   A N D   A S S E M B L I N G
################################################################################

print("--- COMPILING AND ASSEMBLING ---\n");

$indent="   ";

# LOOPING THROUGH OBJECT FILES

@OBJS=();
$total_errors=0;
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
   debug("src_base=[$src_base]");
   debug("src_dir=[$src_dir]");
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
         # -c for compile to object code
         # -l for interspersed C/asm listings
         # -e for C++ comments
         # -I for include file directories
         # -v for verbose
         print($indent."COMPILING...");
         if($debug) { print("\n"); }
         $errors|=run("icc11 -c -l -e -I. -o $obj_path $src_path");
         # actually runs the following
         #icpp.exe -D_HC11 -I. -DICC -e app\\test.c test.i
         #iccom11.exe -lapp\\test.c test.i test.s
         #ias6811.exe -o obj\\test.o test.s
         #rm test.i
         $temp=chop_extension($src_name);
         move_file("$temp.lis",$OBJ_DIR);
         print("OK\n");
      } elsif( $src_ext eq "asm" ) {
         print($indent."ASSEMBLING...");
         if($debug) { print("\n"); }
         $errors|=run("ias6811 -o $obj_path $src_path");
         move_file(chop_extension($src_path).".lis",$OBJ_DIR);
         print("OK\n");
      } else {
         print("HUH?\n");
      }
   } else {
      print("OK\n");
   }

   print("\n");

   # RECORD THE FILES THAT WE NEED TO LINK TOGETHER

   push(@OBJS,$obj_path);

   if($errors gt 0) {
      print("\n");
   }
   $total_errors+=$errors;

}

# END OF --- COMPILING AND ASSEMBLING ---

print("\n");


################################################################################
#   L I N K I N G
################################################################################

# -bdata:0x0000.0x1111:0x2222.0x3333 for multiple zones
# define heap_size for crt11.o to allocate heap area
# define init_sp for your initial stack pointer
# -m option to produce map file

#         MEMORY MAP
# ------------------------
#        DATA   8000-97FF    6k      GLOBAL VARIABLES
#         BSS   9800-AFFF    6k      INITIALIZED GLOBAL VARIABLES
#       STACK   B000-B5FF  1536      INITIAL STACK
#     NOTHING   B600-B7FF   512      EEPROM
#     NOTHING   B800-BFBF   ~2k      NOTHING
# INT VECTORS   BFC0-BFFF    64      SPECIAL INTERRUPTS (HANDYBOARD)
#        TEXT   C000-FFBF  ~16k      CODE INSTRUCTIONS, CONSTANT DATA
# INT VECTORS   FFC0-FFFF    64      NORMAL INTERRUPTS (SIMULATOR)

if($total_errors gt 0) {
   print("errors compiling, skipping the link stage\n");
   print("\n");
} else {
   print("--- LINKING ---\n");
   print("setting up link map and linker options...");
   setenv("ICC11_LINKER_OPTS",
      "-bdata:0x8000.0x97ff ".
      "-bbss:0x9800.0xafff ".
      "-btext:0xc000.0xffbf ".
      "-dinit_sp:0xb5ff ".
      "-dheap_size:0x0 ".
      "-m ");
   print("OK\n");
   print("creating linker parm file...");
   open(TEMP1,">BUILD.TMP");
   print TEMP1 "-L$icc11_lib -o $TARGET.S19 @OBJS";   # TODO - LIBS
   close(TEMP1);
   print("OK\n");
   print("linking...");
   if($debug) { print("\n"); }
   run("icc11 \@BUILD.TMP");
   print("OK\n");
   print("moving map/list files to obj dir...");
   if($debug) { print("\n"); }
   run("move *.lst *.mp $OBJ_DIR >&> NUL:");
   print("OK\n");
   print("cleaning up...");
   unlink("BUILD.TMP");
   print("OK\n");

   # END OF --- LINKING ---

   print("\n");

   print("--- SHOW THE RESULTS ---\n");
   run("dir *.s19");

   # END OF --- SHOW THE RESULTS ---

   print("\n");

}

################################################################################
#   C L E A N   U P
################################################################################

print("--- CLEAN UP ---\n");
print("removing intermediate files...");
unlink <*.aaa>;
unlink <*.aaa>;
unlink <*.s>;
unlink <*.i>;
print("OK\n");

# END OF --- CLEAN UP ---

print("\n");

################################################################################

print("done\n");

#END

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

