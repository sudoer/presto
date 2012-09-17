
################################################################################

# PROJECT CONFIGURATION

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

################################################################################

# COMPILER CONFIGURATION
$compiler_home="c:\\alan\\play\\hc11\\icc";
$ICC11_PATH="$compiler_home\\BIN";
$ICC11_LIB="$compiler_home\\LIB";
$ICC11_INCLUDE="$compiler_home\\INCLUDE";

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

$ICC11_LINKER_OPTS="-bdata:0x8000.0x97ff ".
                   "-bbss:0x9800.0xafff ".
                   "-dinit_sp:0xb5ff ".
                   "-btext:0xc000.0xffbf ".
                   "-dheap_size:0x0 ".
                   "-m ";

################################################################################
#   YOU DON'T NEED TO MESS WITH ANYTHING BELOW THIS LINE
################################################################################

print("\n");
print("--- CONFIGURATION ---\n");
$path=tolower($ENV{PATH});
if(index($ICC11_PATH,$path)<0) {
   $ENV{PATH}=$ENV{PATH}.";$ICC11_PATH";
}
print("path is $ENV{PATH}\n");

print("--- CLEANING UP ---\n");

unlink <*.bak>;
unlink <*.aaa>;
unlink <*.s>;
unlink <*.i>;
#rem del /s/y/x/q %OBJ_DIR% >&> NUL:
#rem del /s/q *.s19 >&> NUL:

print("\n");
print("--- PREPARING ---\n");

mkdir($OBJ_DIR,0777);

print("\n");
print("--- COMPILING AND ASSEMBLING ---\n");

$ENV{"ICC11_INCLUDE"}=$ICC11_INCLUDE;

@OBJS=();
foreach $filepath (@SRC_FILES) {
   print("FILE $filepath...");
   $filename=$filepath;
   $filename=~s/^.*\\//g;
   $filename=~s/^.*\///g;
   $objname=$filename;
   $objname=~s/\.asm$/\.o/g;
   $objname=~s/\.c$/\.o/g;
   $objpath="$OBJ_DIR/$objname";
   $ext=$filepath;
   $ext=~s/^.*\.//g;
   $ext=~tr/A-Z/a-z/;
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
         # -c for compile to object code
         # -l for interspersed C/asm listings
         # -e for C++ comments
         system("icc11 -c -l -e -I. -o $objpath $filepath");
         # if errorlevel 1 goto abort
      } elsif( $ext eq "asm" ) {
         print("ASSEMBLING\n");
         system("ias6811 -o $objpath $filepath");
         # if errorlevel 1 goto abort
         # system("move /q %@PATH[%file%]\%@NAME[%file%].lis %OBJ_DIR%");
      } else {
         print("HUH?\n");
      }
   } else {
      print("OK\n");
   }
   push(@OBJS,$objpath);
}


print("\n");
print("--- LINKING ---\n");

$ENV{"ICC11_LIB"}=$ICC11_LIB;
$ENV{"ICC11_LINKER_OPTS"}=$ICC11_LINKER_OPTS;

open(TEMP1,">BUILD.TMP");
print TEMP1 "-L$ICC11_LIB -o $TARGET.S19 @OBJS";
#print "-L$ICC11_LIB -o $TARGET.S19 @OBJS";
close(TEMP1);
system("icc11 \@BUILD.TMP");
unlink("BUILD.TMP");

print("\n");
print("--- SHOW THE RESULTS ---\n");

system("dir *.s19");

print("\n");
print("--- CLEAN UP ---\n");

unlink <*.aaa>;
unlink <*.aaa>;
unlink <*.s>;
unlink <*.i>;
system("move *.lis *.lst *.mp $OBJ_DIR >&> NUL:");
#END

################################################################################

sub tolower {
   $string=$_[0];
   $string=~tr/A-Z/a-z/;
   return $string;
}

################################################################################

sub toupper {
   $string=$_[0];
   $string=~tr/a-z/A-Z/;
   return $string;
}

################################################################################

