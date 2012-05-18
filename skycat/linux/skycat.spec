Summary: FITS Image Display and Catalog Search Tool for Astronomy
Name: skycat
Version: 3.1.2
Release: 1
License: GNU General Public License
Group: Applications/Engineering
URL: http://archive.eso.org/skycat/
Source0: %{name}-%{version}.tar.gz
Requires: tcl, tk, itcl, tclx, blt, tkimg
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
	--exec-prefix=%_prefix \
	--libdir=%_libdir \
	--mandir=%_mandir \
	--with-tcl=%_libdir \
	--with-tk=%_libdir
make

%install
cd build
make install DESTDIR=%buildroot

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc CHANGES COPYING README
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
%{_libdir}/skycat3.1.2
%{_libdir}/libtclutil2.1.0.so
%{_libdir}/libastrotcl2.1.0.so
%{_libdir}/librtd3.2.1.so
%{_libdir}/libcat4.1.0.so
%{_libdir}/libskycat3.1.2.so
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
* Mon Jan 30 2006 Allan Brighton <abrighto@eso.org> - 
- Initial rpm build.

