
# Pass something like RELEASE=216010f.fc20 to define your own Release
IF(NOT DEFINED RELEASE)
SET(RELEASE "${CMAKE_SYSTEM_NAME}")
ENDIF(NOT DEFINED RELEASE)

SET(CPACK_PACKAGE_NAME Recorder)
SET(CPACK_PACKAGE_VERSION 1.0.0)
SET(CPACK_PACKAGE_VENDOR "Schneider Electric")
SET(CPACK_PACKAGE_GROUP "Multisight-Gateway")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BAMF based video recorder.")

SET(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

IF(CMAKE_SYSTEM MATCHES "Linux-")

  LIST(APPEND CPACK_GENERATOR RPM)
  SET(CPACK_RPM_PACKAGE_NAME Recorder)
  SET(CPACK_RPM_PACKAGE_RELEASE ${RELEASE})
  SET(CPACK_RPM_SPEC_MORE_DEFINE "%define ignore \#") # work around for cpack issue
  SET(CPACK_RPM_USER_FILELIST "%ignore /usr" "%ignore /usr/local" "%ignore /usr/local/Pelco" "%ignore /lib" "%ignore /lib/systemd" "%ignore /lib/systemd/system" "%ignore /etc" "%ignore /etc/rsyslog.d")
  SET(CPACK_RPM_USER_BINARY_SPECFILE "${CMAKE_CURRENT_SOURCE_DIR}/custom.spec")
  SET(CPACK_RPM_PACKAGE_AUTOREQPROV " no")
  SET(CPACK_RPM_PACKAGE_ARCHITECTURE ${TARGET_ARCH})

ELSE(CMAKE_SYSTEM MATCHES "Linux-")

  # Windows specific packaging stuff here...

ENDIF(CMAKE_SYSTEM MATCHES "Linux-")

# Finally, setup our package file name
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${RELEASE}.${TARGET_ARCH}" )

INCLUDE(CPack)
