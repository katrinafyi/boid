install(
    TARGETS tower_exe
    RUNTIME COMPONENT tower_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
