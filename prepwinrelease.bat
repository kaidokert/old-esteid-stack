del /s *.pdb *.ilk

rmdir /s /q debug\et debug\ru
mkdir debug\ET
mkdir debug\RU
copy po\et.gmo debug\ET\EsteidUtil.mo
copy po\ru.gmo debug\RU\EsteidUtil.mo
copy po\et.po debug\et\
copy po\ru.po debug\ru\

rmdir /s /q debugunicode\et debugunicode\ru
mkdir debugunicode\ET
mkdir debugunicode\RU
copy po\et.gmo debugunicode\ET\EsteidUtil.mo
copy po\ru.gmo debugunicode\RU\EsteidUtil.mo
copy po\et.po debugunicode\et\
copy po\ru.po debugunicode\ru\

rmdir /s /q releaseunicode\et releaseunicode\ru
mkdir releaseunicode\ET
mkdir releaseunicode\RU
copy po\et.gmo releaseunicode\ET\EsteidUtil.mo
copy po\ru.gmo releaseunicode\RU\EsteidUtil.mo
copy po\et.po releaseunicode\et\
copy po\ru.po releaseunicode\ru\

rmdir /s /q release\et release\ru
mkdir release\ET
mkdir release\RU
copy po\et.gmo release\ET\EsteidUtil.mo
copy po\ru.gmo release\RU\EsteidUtil.mo
copy po\et.po release\et\
copy po\ru.po release\ru\

7z a -tzip reliis.zip release releaseunicode ChangeLog
rem for %%i in (`grep PACKAGE_VERS src/setup.h | sed "s/[^0-9\.]//g"`) do echo %%i
