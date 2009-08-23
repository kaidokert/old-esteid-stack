echo "EXTRA_DIST = \\" > Makefile.am
find -mindepth 2 -maxdepth 2 -not -path "*.svn*" -type f | sort | sed "s/\(.*\)/\1 \\\/" >> Makefile.am
echo " regen.sh " >> Makefile.am
