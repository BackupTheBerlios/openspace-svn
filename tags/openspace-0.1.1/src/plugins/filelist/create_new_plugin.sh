name=$1
if [ "$1" == "" ]
then
	echo "how to use: "
	echo "sh create_new_plugin.sh plugin_name"
	exit
fi

rm -f "filelist_$name.h"
echo "#ifndef FILELIST_$name" >> "filelist_$name.h"
echo "#define FILELIST_$name" >> "filelist_$name.h"
echo '#include "../../filelist_base.h"'  >> "filelist_$name.h" 
echo "class filelist_$name : public filelist_base { public:"  >> "filelist_$name.h"
export pluginname="filelist_$name";
 
awk '{ if($1=="virtual") {print $2,ENVIRON["pluginname"],"::",$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15\
,$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,$30 } }' ../../filelist_base.h >> "filelist_$name.h"
sed -e 's/ = 0;/;/' "filelist_$name.h" > tmpfile
cp  tmpfile  "filelist_$name.h"
rm -f tmpfile
echo "}; #endif" >> "filelist_$name.h"
indent -l300 -bl -bli0 -bls -cli0 -i4 -nbad -nbap -di1 -nbfda  -npsl "filelist_$name.h"

rm -f "filelist_$name.cpp"

echo '#ifdef WIN32' >> "filelist_$name.cpp"
echo '#define EXPORTFUNCTION extern "C" __declspec(dllexport)' >> "filelist_$name.cpp"
echo '#else' >> "filelist_$name.cpp"
echo '#define EXPORTFUNCTION extern "C"' >> "filelist_$name.cpp"
echo '#endif' >> "filelist_$name.cpp"

echo "#include \"filelist_$name.h\"" >> "filelist_$name.cpp"

 
awk '{ if($1=="virtual") {print $2,ENVIRON["pluginname"],"::",$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15\
,$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,$30 } }' ../../filelist_base.h >> "filelist_$name.cpp"
sed -e 's/ = 0;/{}/' "filelist_$name.cpp" > tmpfile
sed -e 's/ = 0//' tmpfile > "filelist_$name.cpp" 

rm -f tmpfile
echo "EXPORTFUNCTION filelist_base *get_filelist (void){    FXTRACE ((5, \"PLUGIN LOAD\n\"));return new filelist_$name ();}" >> "filelist_$name.cpp"
indent -l300 -bl -bli0 -bls -cli0 -i4 -nbad -nbap -di1 -nbfda  -npsl "filelist_$name.cpp"


export pluginname="libfilelist$name.la";

grep "libfilelist$name.la" Makefile.am > /dev/null
 
#if [ $? ]
#then
#echo "already in Makefile"
#exit
#fi

awk '{ if($1=="pkglib_LTLIBRARIES=") {print $0, ENVIRON["pluginname"] } else print}' Makefile.am > tmpfile
cp tmpfile Makefile.am
rm -f tmpfile

echo "libfilelist$name""_la_LDFLAGS = \`WANT_FOX="1.4" fox-config --libs\` \`xml2-config  --libs\`" >> Makefile.am
echo "libfilelist$name""_la_SOURCES =filelist_$name.cpp filelist_$name.h OSFXFile.cpp ../../thread_elem.cpp ../../thread_elem.h ../../configure.cpp ../../configure.h " >> Makefile.am



