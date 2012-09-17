
$debug=1;

################################################################################
#   P I C K   A   C O M P I L E R
################################################################################

$icc="ICC";
$gcc="GCC";

# pick the compiler here
$compiler=$gcc;

if($compiler eq $icc) {
   print("using the ImageCraft C compiler\n");
} elsif($compiler eq $gcc) {
   print("using the GNU C compiler\n");
}
print("\n");

################################################################################
#   P R O J E C T   C O N F I G U R A T I O N
################################################################################

print("--- CONFIGURATION ---\n");

# SETTING UP THE PROJECT

print("setting up project...");
$OBJ_DIR="obj";
$TARGET="hbtest";
@SRC_FILES=("kernel\\kernel.c",
            "kernel\\task_sw.asm",
            "kernel\\clock.c",
            "kernel\\system.c",
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
if($compiler eq $icc) {
   $compiler_home="c:\\programs\\hc11\\icc";
   add_to_path("$compiler_home\\BIN");
   setenv("ICC11_INCLUDE","$compiler_home\\INCLUDE");
   setenv("DOS4G","quiet");
   setenv("DOS16M","0");
} elsif($compiler eq $gcc) {
   $compiler_home="c:\\programs\\hc11\\gcc";
   add_to_path("$compiler_home\\BIN");
   $gcc_include="$compiler_home\\include";
}
print("OK\n");

print("setting up assembler...");
if($compiler eq $icc) {
} elsif($compiler eq $gcc) {
   # -g undefined symbols made global
   # -a all user symbols made global
   # -l create a list output
   # -o create an object output
   # -s create a symbol output
   # -p disable listing pagination
   # -w wide listing format for symbol table
   # -z enable case-sensitivity for symbols
   $gnu_asm_opts="-loszgp";
}
print("OK\n");




print("setting up linker...");

if($compiler eq $icc) {
   $icc11_lib="$compiler_home\\LIB";
   setenv("ICC11_LIB",$icc11_lib);
} elsif($compiler eq $gcc) {
   $gcc_lib="$compiler_home\\lib";
}
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

chop($base_dir=`cd`);
$base_dir=~s/^[A-Za-z]://g;  # remove C:

# END OF --- PREPARING ---

print("\n");

################################################################################
#   C O M P I L I N G   A N D   A S S E M B L I N G
################################################################################

print("--- COMPILING AND ASSEMBLING ---\n");

# LOOPING THROUGH OBJECT FILES

@OBJS=();
$total_errors=0;
foreach $src_path (@SRC_FILES) {
   print("FILE $src_path...");
   $errors=0;

   # DETERMINE FILE NAMES, BASE NAMES, EXTENSIONS, PATHS

   $src_dir=directory_of($src_path);
   $src_name=basename($src_path);
   $src_base=chop_extension($src_name);
   $src_ext=extension_of($src_path);
   if($debug) {
      print("\n");   # break out of line
      print("src_name=[$src_name]\n");
      print("src_path=[$src_path]\n");
      print("src_base=[$src_base]\n");
      print("src_dir=[$src_dir]\n");
      print("src_ext=[$src_ext]\n");
   }

   if($compiler eq $icc) {
      $obj_name="$src_base.o";
   } elsif($compiler eq $gcc) {
      $obj_name="$src_base.rel";
   }

   $obj_path="$OBJ_DIR\\$obj_name";
   if($debug) {
      print("obj_name=[$obj_name]\n");
      print("obj_path=[$obj_path]\n");
   }

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
         print("COMPILING\n");
         if($compiler eq $icc) {
            # -c for compile to object code
            # -l for interspersed C/asm listings
            # -e for C++ comments
            # -I for include file directories
            $errors|=run("icc11 -c -l -e -I. -D$compiler -o $obj_path $src_path");
         } elsif($compiler eq $gcc) {
            $include_dir=unix_slashes($base_dir);
            $src_dir=directory_of($src_path);
            print("src_dir=[$src_dir]\n");
            print("cd to [$base_dir\\$src_dir]\n");
            chdir("$base_dir\\$src_dir");
            $temp=chop_extension($src_name).".s";
            $errors|=run("xgcc -S -O2 -mlong_branch -I$include_dir -D$compiler -Wa,$gnu_asm_opts -o $temp $src_name");
         }
      } elsif( $src_ext eq "asm" ) {
         print("ASSEMBLING\n");
         if($compiler eq $icc) {
            $errors|=run("ias6811 -o $obj_path $src_path");
            move_file(chop_extension($src_path).".lis",$OBJ_DIR);
         } elsif($compiler eq $gcc) {
            $errors|=run("as6811 $gnu_asm_opts $src_path");
            move_file(chop_extension($src_path).".lst",$OBJ_DIR);
            move_file(chop_extension($src_path).".rel",$OBJ_DIR);
            move_file(chop_extension($src_path).".sym",$OBJ_DIR);
         }
      } else {
         print("HUH?\n");
      }
   } else {
      print("OK\n");
   }

   if($debug) {
      print("\n");
   }

   # RECORD THE FILES THAT WE NEED TO LINK TOGETHER

   push(@OBJS,$obj_path);

   if($errors gt 0) {
      print("\n");
   }
   $total_errors+=$errors;

}

# MOVING LIST FILES TO OBJ DIR

print("moving list files to obj dir...");
run("move *.lis $OBJ_DIR >&> NUL:");
print("OK\n");

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
# INT VECTORS   BFC0-BFFF    64      SPECIAL INTERRUPTS
#        TEXT   C000-FFBF  ~16k      CODE INSTRUCTIONS, CONSTANT DATA
# INT VECTORS   FFC0-FFFF    64      NORMAL INTERRUPTS

if($total_errors gt 0) {
   print("errors compiling, skipping the link stage\n");
   print("\n");
} else {
   print("--- LINKING ---\n");
   if($compiler eq $icc) {
      print("setting up link map and linker options...");
      setenv("ICC11_LINKER_OPTS",
         "-bdata:0x8000.0x97ff ".
         "-bbss:0x9800.0xafff ".
         "-btext:0xc000.0xffbf ".
         "-dinit_sp:0xb5ff ".
         "-dheap_size:0x0 ".
         "-m ");
      print("creating linker parm file...");
      open(TEMP1,">BUILD.TMP");
      print TEMP1 "-L$icc11_lib -o $TARGET.S19 @OBJS";   # TODO - LIBS
      close(TEMP1);
      print("OK\n");
      print("linking...");
      run("icc11 \@BUILD.TMP");
      print("OK\n");
      print("moving map/list files to obj dir...");
      run("move *.lst *.mp $OBJ_DIR >&> NUL:");
      print("OK\n");
      print("cleaning up...");
      unlink("BUILD.TMP");
      print("OK\n");
   } elsif($compiler eq $gcc) {
      # -c/f command line/file mode
      # -p/n enable/disable echo of file.lnk to stdout
      # -i/s intel hex or motorola s19 format
      # -z symbol names are case-sensitive
      # -m generate map file
      # -w wide listing for map file
      # -x/d/q use hex/decimal/octal for map file
      # -u generate updated listing (file.rst) after relocation
      # -b area = expression
      # -g symbol = expression
      # -k library path
      # -l library file
      # -e end of input to linker
      print("creating linker parm file with map and options...");
      chdir($OBJ_DIR);
      open(TEMP1,">$TARGET.lnk");
      print TEMP1 "-mszu\n";
      print TEMP1 "-bROMCODE=\n";
      print TEMP1 "-b_LOADER=\n";
      print TEMP1 "-b_BSS=\n";
      print TEMP1 "-b_CODE=\n";
      print TEMP1 "-bSTACK=\n";
      print TEMP1 "-bABS=\n";
      print TEMP1 "-g heap_size=0x0\n";
      print TEMP1 "-k $gcc_lib\\\n";
      print TEMP1 "-k .\\$OBJ_DIR\\\n";
      print TEMP1 "-l system\n";
      foreach $obj (@OBJS) {
         #print TEMP1 $obj." ";
         print TEMP1 chop_extension(basename($obj))."\n";
      }
      print TEMP1 "-e\n";
      close(TEMP1);
      print("OK\n");
      print("linking...");
      run("aslink -f $TARGET");                                # TODO - LIBS
      print("OK\n");

      print("cleaning up...");
      unlink("$TARGET.lnk");
      print("OK\n");
   }

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

sub run {
   my $cmd=$_[0];
   my $rc;
   print("RUNNING [$cmd]\n");
   $rc=system($cmd);
   return $rc;
}

################################################################################

sub move_file {
   my $src=$_[0];
   my $dest=$_[1];
   run("move $src $dest > NUL:");
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


