VERSION=`grep PACKAGE_VERSION src/Setup.h | sed -e 's/.* \"//' -e 's/\"//'`
echo $VERSION

rm -rf deback
mkdir deback
cp -f esteidutil-$VERSION.tar.gz deback/esteidutil_$VERSION.orig.tar.gz
cd deback
tar xvzf esteidutil_$VERSION.orig.tar.gz
cd esteidutil-$VERSION
pdebuild --auto-debsign
lintian -i /var/cache/pbuilder/result/*$VERSION*.changes 
linda -i /var/cache/pbuilder/result/*$VERSION*.changes
