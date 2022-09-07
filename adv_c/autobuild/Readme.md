#https://thoughtbot.com/blog/the-magic-behind-configure-make-make-install
#for autotools
sudo apt-get install autotools-dev
#for autoconf
sudo apt-get install autoconf
#create autoconf.ac for generating autoconf.am, Makefile.am for generating Makefile.in (template)
aclocal
autoconf
automake --add-missing
#The end user doesn’t need to see our autotools setup, so we can distribute the configure script and Makefile.in without all of the files we used to generate them.

./configure
make dist
make distcheck

./configure # Generate Makefile from Makefile.in
make # Use Makefile to build the program
make install # Use Makefile to install the program
-----------------------------------------------------------------------
On maintainer's system:
aclocal # Set up an m4 environment
autoconf # Generate configure from configure.ac
automake --add-missing # Generate Makefile.in from Makefile.am
./configure # Generate Makefile from Makefile.in
make distcheck # Use Makefile to build and test a tarball to distribute


On user's system:
./configure # Generate Makefile from Makefile.in
make # Use Makefile to build the program
make install # Use Makefile to install the program
