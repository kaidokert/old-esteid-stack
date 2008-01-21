#!/bin/bash -x

rm -rf repo
mkdir repo 
cd repo
mkdir binary source
cp -v /var/cache/pbuilder/result/*.deb binary/
cp -v /var/cache/pbuilder/result/*.gz /var/cache/pbuilder/result/*.dsc source/

dpkg-scanpackages binary /dev/null | gzip -9c > binary/Packages.gz
dpkg-scansources source /dev/null | gzip -9c > source/Sources.gz

echo Archive: unstable > Release
echo Component: main >> Release
echo Origin: Kaido Kert >> Release
echo Label: Esteid smartcard >> Release
cp Release source/Release
cp Release binary/Release
echo Architecture: i386 >> binary/Release
echo Architecture: source >> source/Release

apt-ftparchive release binary >> binary/Release
apt-ftparchive release source >> source/Release

apt-ftparchive contents binary > Contents-i386
apt-ftparchive release . >> Release
gpg --output Release.gpg -ba Release

if [[ $1 = 'upload' ]] ; then
	lftp -f ../upload.cmd
fi
