## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
## # The following are required to use Dart and CDash
set(CTEST_PROJECT_NAME "OpenStudio")
set(CTEST_NIGHTLY_START_TIME "00:00:00 MST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "my.cdash.org")
set(CTEST_DROP_LOCATION "/submit.php?project=OpenStudio")
set(CTEST_DROP_SITE_CDASH TRUE)
#set(CTEST_USE_LAUNCHERS 1) # Wraps all build and test processes so that more detailed reports can be pushed to CDash

# no memory check suppressions
set(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE "")

# custom src excludes and exclude generated code and unit tests
set(CTEST_CUSTOM_COVERAGE_EXCLUDE
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/boost-log"
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/clips"
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/expat"
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/gen_iddfactory"
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/gtest"
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/libssh"
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/litesql"
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/qwt"
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/utilities/sql/Sqlite3.h"
  "${CTEST_SOURCE_DIRECTORY}/openstudiocore/src/utilities/sql/Sqlite3.c"
  "${CTEST_BUILD_DIRECTORY}"
  "hxx$"
  "cxx$"
  "Test.h$"
  "Test.hpp$"
  "Test.cpp$"
  "Fixture.h$"
  "Fixture.hpp$"
  "Fixture.cpp$"
)

set(CTEST_CUSTOM_WARNING_EXCEPTION
  ${CTEST_CUSTOM_WARNING_EXCEPTION}
  ".*/VTK-prefix/src/VTK/.*"
  ".*/src/libssh/.*"
  ".*/src/sqlite/.*"
  ".*/src/litesql/.*"
  ".*warning: variable 'klass' set but not used.*"
)
