# -*- mode: cmake; -*-

# create install tree for chumy usb-image
#
# tree is:
# usb/
#    /chumby-controlpanel
#    /lib
#    /etc
#    /plugins
#    /translations

function(makeChumbyImage)
  set(_dir ${CMAKE_INSTALL_PREFIX})

  file(MAKE_DIRECTORY ${_dir})
  # cp recursive ${QT_PLUGINS_DIR} to ${_dir}/plugins
  execute_process(COMMAND cp -r ${QT_PLUGINS_DIR} ${_dir}/)

  # cp alsa-sound
  install execute_process(COMMAND cp -r ${QT_PLUGINS_DIR} ${_dir}/)

  # cp tslib (touch screen lib)


endfunction(makeChumbyImage)
