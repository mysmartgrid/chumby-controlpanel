# -*- mode: cmake; -*-

#set(_dir ${CMAKE_INSTALL_PREFIX})
get_filename_component(QT_LIBRARY_DIR "${QT_QTCORE_LIBRARY}" PATH)

# create file debugchumby
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/debugchumby.in ${CMAKE_CURRENT_BINARY_DIR}/debugchumby )

# create file env.sh
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/env.sh.in ${CMAKE_CURRENT_BINARY_DIR}/env.sh )

# ts.conf
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ts.conf.in ${CMAKE_CURRENT_BINARY_DIR}/ts.conf )

# flukso.conf
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/flukso.conf.in ${CMAKE_CURRENT_BINARY_DIR}/flukso.conf )


install(PROGRAMS debugchumby DESTINATION .)
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/start_network DESTINATION .)
install(FILES    ${CMAKE_CURRENT_BINARY_DIR}/env.sh DESTINATION .)
install(FILES    ${CMAKE_CURRENT_BINARY_DIR}/flukso.conf DESTINATION .)
#install(FILES    ${CMAKE_CURRENT_BINARY_DIR}/update.tgz DESTINATION .)

install(FILES    ${CMAKE_CURRENT_BINARY_DIR}/ts.conf DESTINATION etc)

install(DIRECTORY ${QT_PLUGINS_DIR}/ DESTINATION plugins)

install_chumby_qt4()
#install(DIRECTORY ${QT_LIBRARY_DIR}/ DESTINATION lib FILES_MATCHING PATTERN "*.so*")
install_chumby_toolchain()
#install(DIRECTORY ${TOOLCHAIN_PREFIX}/lib/ DESTINATION lib FILES_MATCHING PATTERN "*.so*")

install(DIRECTORY ${QT_LIBRARY_DIR}/fonts DESTINATION lib/)
install(DIRECTORY ${LIBTS_PLUGIN_DIR} DESTINATION lib)
install(PROGRAMS  ${TOOLCHAIN_PREFIX}/bin/ts_calibrate DESTINATION bin)

#install(FILES   ${TOOLCHAIN_PREFIX}/lib/libasound.so DESTINATION ${_dir}/lib NAMELINK_SKIP)
#install(FILES   ${TOOLCHAIN_PREFIX}/lib/libts.so DESTINATION ${_dir}/lib NAMELINK_SKIP)
#install(FILES   ${TOOLCHAIN_PREFIX}/lib/libiw.so DESTINATION ${_dir}/lib NAMELINK_SKIP )
#install(FILES ${ASOUND_LIBRARY} DESTINATION ${_dir}/lib)
#install(FILES ${LIBTS_LIBRARY} DESTINATION ${_dir}/lib)
