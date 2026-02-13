if (NOT DEFINED BUNDLE_PATH)
  set(BUNDLE_PATH "bundle")
endif()

add_custom_target(bundle_install ALL
  COMMAND ${BUNDLE_PATH} install
  
  COMMENT "Running bundle install"
)
