# -*- mode: cmake; -*-

# chumby install
function(install_chumby_qt4)
  message(">>${QT_LIBRARY_DIR}<==")
  file(GLOB_RECURSE IN RELATIVE ${QT_LIBRARY_DIR} _RECURSE "${QT_LIBRARY_DIR}/*.so*" )
  #file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/install.sh "#\n")
  message(">>${IN}<==")
  foreach (it ${IN})
    execute_process(COMMAND cmake -E copy_if_different ${QT_LIBRARY_DIR}/${it} ${CMAKE_CURRENT_BINARY_DIR}/lib/${it})
    
    #file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/install.sh 
    #  "execute_process(COMMAND cmake -E copy ${QT_LIBRARY_DIR}/${it} ${CMAKE_INSTALL_PREFIX}/lib/${it})\n")
  endforeach()
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/ DESTINATION lib)

  #install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/install.sh)
  #install(CODE
  #  "execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/install.sh)")
endfunction(install_chumby_qt4)


# chumby install
function(install_chumby_toolchain)
  set(_libdir ${TOOLCHAIN_PREFIX}/lib)
  set(_dest   ${CMAKE_CURRENT_BINARY_DIR}/lib)
  execute_process(COMMAND cmake -E copy_if_different ${_libdir}/libts.so ${_dest}/${it})
  execute_process(COMMAND cmake -E copy_if_different ${_libdir}/libts-1.0.so.0 ${_dest}/${it})
  execute_process(COMMAND cmake -E copy_if_different ${_libdir}/libiw.so ${_dest}/${it})

endfunction()
