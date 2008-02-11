#!/bin/bash -x
./configure --with-wx-config=/home/kaido/Projects/wxWidgets-2.8/staticinst/bin/wx-config
make clean
make -C po/ update-po
make -C po/ update-gmo
make

cd src/cardlib/ && doxygen && zip -r ../../cardlib_basic_docs.zip html && cd ../..

cp src/EsteidUtil .
rm -rf et ru
mkdir -p et ru
cp po/et.po et
cp po/ru.po ru
cp po/et.gmo et/EsteidUtil.mo
cp po/ru.gmo ru/EsteidUtil.mo

tar cvzf EsteidUtil_`grep PACKAGE_VERSION src/Setup.h | sed -e 's/.* \"//' -e 's/\"//'`_Linux86Static.tar.gz \
	EsteidUtil et/ ru/ ChangeLog cardlib_basic_docs.zip
