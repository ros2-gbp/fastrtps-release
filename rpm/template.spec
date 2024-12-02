%bcond_without tests
%bcond_without weak_deps

%global __os_install_post %(echo '%{__os_install_post}' | sed -e 's!/usr/lib[^[:space:]]*/brp-python-bytecompile[[:space:]].*$!!g')
%global __provides_exclude_from ^/opt/ros/jazzy/.*$
%global __requires_exclude_from ^/opt/ros/jazzy/.*$

Name:           ros-jazzy-fastrtps
Version:        2.14.4
Release:        1%{?dist}%{?release_suffix}
Summary:        ROS fastrtps package

License:        Apache 2.0
URL:            https://www.eprosima.com/
Source0:        %{name}-%{version}.tar.gz

Requires:       openssl-devel
Requires:       python%{python3_pkgversion}-devel
Requires:       ros-jazzy-fastcdr
Requires:       ros-jazzy-foonathan-memory-vendor
Requires:       tinyxml2-devel
Requires:       ros-jazzy-ros-workspace
BuildRequires:  asio-devel
BuildRequires:  cmake3
BuildRequires:  openssl-devel
BuildRequires:  python%{python3_pkgversion}-devel
BuildRequires:  ros-jazzy-fastcdr
BuildRequires:  ros-jazzy-foonathan-memory-vendor
BuildRequires:  tinyxml2-devel
BuildRequires:  ros-jazzy-ros-workspace
Provides:       %{name}-devel = %{version}-%{release}
Provides:       %{name}-doc = %{version}-%{release}
Provides:       %{name}-runtime = %{version}-%{release}

%description
*eprosima Fast DDS* (formerly Fast RTPS) is a C++ implementation of the DDS
(Data Distribution Service) standard of the OMG (Object Management Group).
eProsima Fast DDS implements the RTPS (Real Time Publish Subscribe) protocol,
which provides publisher-subscriber communications over unreliable transports
such as UDP, as defined and maintained by the Object Management Group (OMG)
consortium. RTPS is also the wire interoperability protocol defined for the Data
Distribution Service (DDS) standard. *eProsima Fast DDS* expose an API to access
directly the RTPS protocol, giving the user full access to the protocol
internals.

%prep
%autosetup -p1

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/jazzy/setup.sh" ]; then . "/opt/ros/jazzy/setup.sh"; fi
mkdir -p .obj-%{_target_platform} && cd .obj-%{_target_platform}
%cmake3 \
    -UINCLUDE_INSTALL_DIR \
    -ULIB_INSTALL_DIR \
    -USYSCONF_INSTALL_DIR \
    -USHARE_INSTALL_PREFIX \
    -ULIB_SUFFIX \
    -DCMAKE_INSTALL_PREFIX="/opt/ros/jazzy" \
    -DCMAKE_PREFIX_PATH="/opt/ros/jazzy" \
    -DSETUPTOOLS_DEB_LAYOUT=OFF \
%if !0%{?with_tests}
    -DBUILD_TESTING=OFF \
%endif
    -DINSTALL_EXAMPLES=OFF \
    -DSECURITY=ON \
    -DAPPEND_PROJECT_NAME_TO_INCLUDEDIR=ON \
    ..

%make_build

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/jazzy/setup.sh" ]; then . "/opt/ros/jazzy/setup.sh"; fi
%make_install -C .obj-%{_target_platform}

%if 0%{?with_tests}
%check
# Look for a Makefile target with a name indicating that it runs tests
TEST_TARGET=$(%__make -qp -C .obj-%{_target_platform} | sed "s/^\(test\|check\):.*/\\1/;t f;d;:f;q0")
if [ -n "$TEST_TARGET" ]; then
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/jazzy/setup.sh" ]; then . "/opt/ros/jazzy/setup.sh"; fi
CTEST_OUTPUT_ON_FAILURE=1 \
    %make_build -C .obj-%{_target_platform} $TEST_TARGET || echo "RPM TESTS FAILED"
else echo "RPM TESTS SKIPPED"; fi
%endif

%files
%license LICENSE
/opt/ros/jazzy

%changelog
* Mon Dec 02 2024 Miguel Company <miguelcompany@eprosima.com> - 2.14.4-1
- Autogenerated by Bloom

* Thu Oct 17 2024 Miguel Company <miguelcompany@eprosima.com> - 2.14.3-1
- Autogenerated by Bloom

* Mon May 13 2024 Miguel Company <miguelcompany@eprosima.com> - 2.14.1-1
- Autogenerated by Bloom

* Thu Apr 18 2024 Miguel Company <miguelcompany@eprosima.com> - 2.14.0-2
- Autogenerated by Bloom

* Tue Mar 26 2024 Miguel Company <miguelcompany@eprosima.com> - 2.14.0-1
- Autogenerated by Bloom

* Thu Mar 07 2024 Miguel Company <miguelcompany@eprosima.com> - 2.13.3-1
- Autogenerated by Bloom

* Thu Mar 07 2024 Miguel Company <miguelcompany@eprosima.com> - 2.13.2-3
- Autogenerated by Bloom

* Wed Mar 06 2024 Miguel Company <miguelcompany@eprosima.com> - 2.13.2-2
- Autogenerated by Bloom

