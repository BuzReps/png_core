# Add/append to documentation generation target `generate_docs`
# Writes docs to ${PROJECT_BINARY_DIR}/${project_dir} directory
# @param project_dir Relative to `PROJECT_SOURCE_DIR` directory from which documentation is generated
function(AppendToDocsTarget project_dir)
  set(docs_target_name generate_docs)

  include(FindDoxygen)
  if (NOT DOXYGEN_FOUND)
    add_custom_target(${docs_target_name}
      COMMAND false
      COMMENT "Doxygen need to be installed to generate the doxygen documentation")
    return()
  endif()

  # CMake creates DOXYGEN_OUTPUT_DIRECTORY,
  # but does not create appended DOXYGEN_*_OUTPUT directory
  # so we will 'mkdir -p' ourselves
  set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/docs)

  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_HTML_OUTPUT ${DOXYGEN_OUTPUT_DIRECTORY}/${project_dir}/html)
  file(MAKE_DIRECTORY ${DOXYGEN_HTML_OUTPUT})

  set(DOXYGEN_GENERATE_MAN YES)
  set(DOXYGEN_MAN_OUTPUT ${DOXYGEN_OUTPUT_DIRECTORY}/${project_dir}/man)
  file(MAKE_DIRECTORY ${DOXYGEN_MAN_OUTPUT})

  doxygen_add_docs(${docs_target_name}
    ${PROJECT_SOURCE_DIR}/${project_dir}
    COMMENT "Generate documentation")
endfunction()

