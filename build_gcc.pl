
print("\n");
print("--- CONFIGURATION ---\n");

################################################################################

# PROJECT CONFIGURATION

print("setting up project...");
$OBJ_DIR="obj";
$TARGET="hbtest";
@SRC_FILES=("presto/kernel.c",
            "presto/task_sw.asm",
            "presto/clock.c",
            "presto/system.c",
            "services/debugger.c",
            "services/lcd.c",
            "services/i2c.c",
            "services/motors.c",
            "services/serial.c",
            "services/sound.c",
            "app/test.c");
print("OK\n");

################################################################################

# COMPILER CONFIGURATION

print("setting up compiler...");
$compiler_home="c:\\programs\\hc11\\gcc";
add_to_path("$compiler_home\\bin");
$gcc_include="$compiler_home\\include";
print("OK\n");

################################################################################

# LINKER CONFIGURATION

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

print("setting up linker...");
setenv("ICC11_LINKER_OPTS",
   "-bdata:0x8000.0x97ff ".
   "-bbss:0x9800.0xafff ".
   "-dinit_sp:0xb5ff ".
   "-btext:0xc000.0xffbf ".
   "-dheap_size:0x0 ".
   "-m ");
$gcc_lib="$compiler_home\\lib";
#setenv("ICC11_LIB",$icc11_lib);
print("OK\n");

################################################################################
#   YOU DON'T NEED TO MESS WITH ANYTHING BELOW THIS LINE
################################################################################

print("\n"); # end of --- CONFIGURATION ---

print("--- PREPARING ---\n");
print("deleting old intermediate files...");
unlink <*.bak>;
unlink <*.aaa>;
unlink <*.s>;
unlink <*.i>;
#rem del /s/y/x/q %OBJ_DIR% >&> NUL:
#rem del /s/q *.s19 >&> NUL:
print("OK\n");
print("creating object directory...");
mkdir($OBJ_DIR,0777);
print("OK\n");
print("\n"); # end of --- PREPARING ---

print("--- COMPILING AND ASSEMBLING ---\n");
@OBJS=();
foreach $filepath (@SRC_FILES) {
   print("FILE $filepath...");
   $filename=basename($filepath);
   $objname=chop_extension($filename).".o";
   $objpath="$OBJ_DIR/$objname";
   $ext=extension_of($filepath);
   #print("\n");
   #print("filename=[$filename]\n");
   #print("objname=[$objname]\n");
   #print("objpath=[$objpath]\n");
   #print("ext=[$ext]\n");
   $work=0;
   if( ! -e "$objpath" ) {
      $work=1;
   } else {
      $filetime= -M $filepath;
      $objtime= -M $objpath;
      #print("\n");
      #print("file time = [$filetime]\n");
      #print("obj time = [$objtime]\n");
      if( $filetime < $objtime ) {
         $work=1;
      }
   }
   if( $work == 1 ) {
      if($ext eq "c") {
         print("COMPILING\n");
         system("xgcc -c -O2 -mlong_branch -S -I. -o $objpath $filepath");
      } elsif( $ext eq "asm" ) {
         print("ASSEMBLING\n");
         system("as6811 -loszg $filepath");
         system("move $objfile $objpath");
      } else {
         print("HUH?\n");
      }
   } else {
      print("OK\n");
   }
   push(@OBJS,$objpath);
}
print("\n"); # end of --- COMPILING AND ASSEMBLING ---

print("--- LINKING ---\n");
print("creating linker parm file...");
$chop=chop_extension($filename)."";
open(TEMP1,">$chop.lnk");
print TEMP1 "-mszu\n";
print TEMP1 "-b_LOADER=0xd000\n";
print TEMP1 "-b_CODE=0x0d003\n";
print TEMP1 "-b_BSS=0x0C000\n";
print TEMP1 "-k $gcc_lib\\\n";
print TEMP1 "-l system\n";
print TEMP1 "%1\n";
print TEMP1 "-e\n";
close(TEMP1);
print("OK\n");
print("linking...");
system("aslink -f $chop");
print("OK\n");
print("cleaning up...");
unlink("$chop.lnk");
print("OK\n");
print("\n"); # end of --- LINKING ---

print("--- SHOW THE RESULTS ---\n");
system("dir *.s19");
print("\n"); # end of --- SHOW THE RESULTS ---

print("--- CLEAN UP ---\n");
print("removing intermediate files...");
unlink <*.aaa>;
unlink <*.aaa>;
unlink <*.s>;
unlink <*.i>;
print("OK\n");
print("moving list files to obj dir...");
system("move *.lis *.lst *.mp $OBJ_DIR >&> NUL:");
print("OK\n");
print("\n"); # end of --- CLEAN UP ---

print("done\n");
#END

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
   $filename="$filename.$ext";
   return $filename;
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


