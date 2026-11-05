# "Must not compile" test (ctest runs it).
# Builds the target TARGET and checks two things:
#   1. the compilation fails,
#   2. the error output contains the text EXPECTED (from the "// EXPECTED_ERROR:" line in FILE).
# Parameters (-D): BUILD_DIR, TARGET, CONFIG, FILE, EXPECTED.

execute_process(
  COMMAND ${CMAKE_COMMAND} --build ${BUILD_DIR} --target ${TARGET} --config ${CONFIG}
  RESULT_VARIABLE result
  OUTPUT_VARIABLE output
  ERROR_VARIABLE output)

if(result EQUAL 0)
  message(FATAL_ERROR
    "FAIL ${FILE}\n"
    "  the code compiled, but it must fail with the error: \"${EXPECTED}\"")
endif()

string(FIND "${output}" "${EXPECTED}" position)
if(position EQUAL -1)
  message("${output}")
  message(FATAL_ERROR
    "FAIL ${FILE}\n"
    "  there is a compilation error (above), but without the message \"${EXPECTED}\"")
endif()

message("OK ${FILE}: compilation error with the message \"${EXPECTED}\"")
