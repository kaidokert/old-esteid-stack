VERSION=`grep PACKAGE_VERSION src/Setup.h | sed -e 's/.* \"//' -e 's/\"//'`
echo $VERSION

rm -rf deback
mkdir deback
cp -f EsteidUtil-$VERSION.tar.gz deback/esteidutil_$VERSION.orig.tar.gz
cd deback
tar xvzf esteidutil_$VERSION.orig.tar.gz
cd EsteidUtil-$VERSION
pdebuild --auto-debsign
lintian -i /var/cache/pbuilder/result/*.changes 
linda -i /var/cache/pbuilder/result/*.changes
