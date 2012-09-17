set VER=105

del ..\presto%VER%.zip
pkzip25 -add -rec -path=rel ..\presto%VER%.zip *.*
pkzip25 -view=brief ..\presto%VER%.zip


