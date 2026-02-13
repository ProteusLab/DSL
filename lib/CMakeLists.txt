project(protea_irgen)

add_custom_command(
  OUTPUT IR.yaml
  COMMAND ${BUNDLE_PATH} exec ruby ${CMAKE_CURRENT_SOURCE_DIR}/ir_gen.rb
  
  DEPENDS bundle_install
  COMMENT "Running ${PROJECT_NAME}"
)
add_custom_target(${PROJECT_NAME} DEPENDS IR.yaml)
