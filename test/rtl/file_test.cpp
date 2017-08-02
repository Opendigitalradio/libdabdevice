#include "file_suites/constants.h"
#include "file_suites/looping_suite.h"
#include "file_suites/normalization_suite.h"
#include "file_suites/option_suite.h"

#include <cute/cute.h>
#include <cute/cute_runner.h>
#include <cute/cute_suite.h>
#include <cute/xml_listener.h>
#include <cute/ide_listener.h>
#include <cutex/descriptive_suite.h>

#include <cstdio>
#include <fstream>
#include <future>
#include <random>

using namespace dab::test::rtl::file;

void setup()
  {
  std::ofstream emptyFile{kEmptyFileName};
  emptyFile.close();

  std::ofstream evenSampleFile{kEvenSampleFileName, std::ios::binary | std::ios::trunc};
  evenSampleFile.write((char *)kEvenSampleData, sizeof(kEvenSampleData));
  evenSampleFile.close();

  std::ofstream oddSampleFile{kOddSampleFileName, std::ios::binary | std::ios::trunc};
  oddSampleFile.write((char *)kOddSampleData, sizeof(kOddSampleData));
  oddSampleFile.close();
  }

void teardown()
  {
  remove(kEmptyFileName);
  remove(kEvenSampleFileName);
  remove(kOddSampleFileName);
  }

int main(int argc, char * * argv)
  {
  auto xmlFile = cute::xml_file_opener{argc, argv};
  auto listener = cute::xml_listener<cute::ide_listener<>>{xmlFile.out};

  auto success = true;
  auto runner = cute::makeRunner(listener, argc, argv);

  setup();
  success &= cute::extensions::runSelfDescriptive<looping_tests>(runner);
  success &= cute::extensions::runSelfDescriptive<normalization_tests>(runner);
  success &= cute::extensions::runSelfDescriptive<option_tests>(runner);
  teardown();

  return !success;
  }

