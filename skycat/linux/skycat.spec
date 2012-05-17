# CG. 14/07/2010
# Add the following to your ~/.rpmmacros
# %_includedir %(echo $TCLTK_ROOT)/include
# %_libdir %(echo $TCLTK_ROOT)/lib
# %_prefix %(echo $TCLTK_ROOT)
#
# Before executing any rpmbuild, define the env TCLTK_ROOT as the
# location where TCL+TK+ITCL+TCLX+BLT+TKIMG is installed, e.g.
#  export TCLTK_ROOT=/opt/tcltk 
#  export TCLTK_ROOT=/vlt/VLT2010/tlctk
#
Summary: FITS Image Display and Catalog Search Tool for Astronomy
Name: skycat
Version: 3.1.1
Release: 1
License: GNU General Public License
Group: Applications/Engineering
URL: http://archive.eso.org/cms/tools-documentation/skycat
Source0: %{name}-%{version}.tar.gz
#Requires: tcl, tk, itcl, tclx, blt, tkimg
# The binary RPM requires the tcltk RPM
Requires: tcltk
BuildRequires: %{_includedir}/tk.h, %{_includedir}/tcl.h
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
SkyCat is a tool that combines visualization of images and access to 
catalogs and archive data for astronomy.
Note: There is also a Java version of Skycat called JSkycat available 
as part of the JSky package.



Authors:
--------
    Allan Brighton (abrighto@eso.org)
    Thomas Herlin (therlin@eso.org)
    Miguel Albrecht (malbrech@eso.org)
    Daniel Durand (durand@dao.nrc.ca)
    Peter Biereichel (pbiereic@eso.org)

%prep
%setup -q -n %name-%version

%build
export CFLAGS="$RPM_OPT_FLAGS -fno-strict-aliasing"
mkdir build
cd build
../configure \
	--prefix=%_prefix \
	--exec_prefix=%_prefix \
	--libdir=%_libdir \
	--mandir=%_mandir \
	--with-tcl=%_libdir \
	--with-tk=%_libdir
make

%install
rm -rf $RPM_BUILD_ROOT
cd build
make install DESTDIR=%buildroot

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
#%doc CHANGES COPYING README
%{_bindir}/skycat
%{_bindir}/rtd
%{_bindir}/tRtd
%{_bindir}/rtdServer
%{_bindir}/rtdClient
%{_bindir}/rtdCubeDisplay
%{_libdir}/tclutil2.1.0
%{_libdir}/astrotcl2.1.0
%{_libdir}/rtd3.2.1
%{_libdir}/cat4.1.0
%{_libdir}/skycat3.1.1
%{_libdir}/libtclutil2.1.0.so
%{_libdir}/libastrotcl2.1.0.so
%{_libdir}/librtd3.2.1.so
%{_libdir}/libcat4.1.0.so
%{_libdir}/libskycat3.1.1.so
%{_libdir}/librtdImgEvt.a
%{_libdir}/librtdRemote.a
%{_libdir}/tclutilConfig.sh
%{_libdir}/astrotclConfig.sh
%{_libdir}/rtdConfig.sh
%{_libdir}/catConfig.sh
%{_libdir}/skycatConfig.sh
%{_includedir}/tclutil
%{_includedir}/astrotcl
%{_includedir}/rtd
%{_includedir}/cat
%{_includedir}/skycat

%changelog
* Thu Jul 14 2010 Carlos Guirao <cguirao@eso.org> -
- Modified and tested with SL53

* Mon Jan 30 2006 Allan Brighton <abrighto@eso.org> - 
- Initial rpm build.

