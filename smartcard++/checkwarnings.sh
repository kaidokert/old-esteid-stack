find -name "*cpp" -exec /usr/lib/gcc-snapshot/bin/g++ -c -Wall -Wextra -pedantic -I. -I.. -I/usr/include/PCSC/ {} \; -print
