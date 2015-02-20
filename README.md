###XSDK
General purpose windows / linux C++ portability library.

XSDK is the root of a family of C++ libraries and build tools. If the XSDK ecosystem has a specialty, it's probably writing media focused server applications and clients.

#####XSDK Features
In general, XSDK provides wrappers to operating system functionality that varies between supported platforms. Amongst other features, XSDK provides utility classes for:

  - Sockets
  - SSL Sockets
  - Date and Time
  - Threads, mutexes, condition variables, etc
  - Basic actor model support via XCommandQueue and XBlockingQueue.
  - Variant types
  - A JSON parser
  - An dom based XML parser
  - MD5 support
  - Reference counting
  - Memory mapping
  - Nullable types
  - File system and large files support
  - Shared libraries
  - Timing
  - Statistics
  - System information (CPU use, disk usage, etc)
  - UDP sockets
  
Some of this is now implemented in the C++ standard library (since C++11) and its use should probably be considered deprecated. But the vast majority of it is still valid and will be supported for some time.

#####Build Infrastructure
A good build infrastructure should be:
  1) Easy to use.
  2) Extensable.
  3) Fast.
  4) Relocatable.

The XSDK build system is based on CMake. The central idea behind the whole system is a shared installation directory called "devel_artifacts". When XSDK itself is built, if a directory at the same level of XSDK called "devel_artifacts" doesn't exist the XSDK build will create it. The XSDK build will then install XSDK's headers into "devel_artifacts/include/XSDK" and XSDK's library into "devel_artifacts/lib". In addition, the XSDK project also installs some build infrastructure files into "devel_artifacts/build".

Code that depends on XSDK falls into two main categories: libraries and applications. If dependant code makes use of the CMake build infrastructure provided by XSDK, then it will reap the benefits of automatically knowing where devel_artifacts is, and automatically having access to any previously installed headers and libraries. Dependant libraries should install their headers into a subdirectory of "devel_artifacts/include" and their libs into "devel_artifacts/lib". Applications built with this build infracture install their packages (currently RPM) into "devel_artifacts/packages".

Finally, to drive builds at the macro level we use a tool called buildy (also open source). The basic idea behind buildy is that you provide it with a JSON document that lists a collection of GIT repositories in build order and it builds them. In addition to driving builds, buildy also wraps some git functionality and allows it to work across multiple git repositories. For example: buildy --status will do a git status in each build component. You do not have to use buildy to use the above describe build infrastructure (but you will probably find you need something to manage your build at the macro level).
