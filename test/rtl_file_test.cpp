#include "device/rtl_file.h"

#include <cute/cute.h>
#include <cute/cute_runner.h>
#include <cute/cute_suite.h>
#include <cute/xml_listener.h>
#include <cute/ide_listener.h>

#include <cstdio>
#include <fstream>
#include <future>
#include <random>

auto constexpr kEmptyFileName = "rtl_file_empty";
auto constexpr kEvenSampleFileName = "rtl_file_even_sample";
auto constexpr kOddSampleFileName = "rtl_file_odd_sample";

std::uint8_t constexpr kEvenSampleData[] = {0, 32, 64, 96, 128, 160, 192, 255};
std::uint8_t constexpr kOddSampleData[] = {0, 32, 64, 96, 128, 160, 192, 224, 255};

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

void test_enable_invalid_option()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kEvenSampleFileName};

  ASSERT(!device.enable(dab::device::option::automatic_gain_control));
  }

void test_enable_valid_option()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kEvenSampleFileName};

  ASSERT(device.enable(dab::device::option::loop));
  }
void test_disable_invalid_option()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kEvenSampleFileName};

  ASSERT(!device.disable(dab::device::option::automatic_gain_control));
  }

void test_disable_valid_option()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kEvenSampleFileName};

  ASSERT(device.disable(dab::device::option::loop));
  }

void test_looping_is_disabled_by_default()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kEvenSampleFileName};
  device.run();

  ASSERT(true);
  }

void test_looping()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kEvenSampleFileName};
  device.enable(dab::device::option::loop);

  auto runner = std::async(std::launch::async, [&]{device.run();});

  std::this_thread::sleep_for(std::chrono::milliseconds{500});
  device.stop();

  runner.get();
  ASSERT(true);
  }

void test_non_looping_even_4_samples()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kEvenSampleFileName};
  device.run();

  dab::__internal_common::sample_t ignored{};
  std::size_t count{};

  while(queue.try_dequeue(ignored))
    {
    ++count;
    }

  ASSERT_EQUAL(4, count);
  }

void test_non_looping_odd_4_samples()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kOddSampleFileName};
  device.run();

  dab::__internal_common::sample_t ignored{};
  std::size_t count{};

  while(queue.try_dequeue(ignored))
    {
    ++count;
    }

  ASSERT_EQUAL(4, count);
  }

void test_looping_more_samples()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kEvenSampleFileName};
  device.enable(dab::device::option::loop);

  auto runner = std::async(std::launch::async, [&]{device.run();});

  std::this_thread::sleep_for(std::chrono::milliseconds{500});
  device.stop();

  runner.get();

  dab::__internal_common::sample_t ignored{};
  std::size_t count{};

  while(queue.try_dequeue(ignored))
    {
    ++count;
    }

  ASSERT_LESS(4, count);
  }

void test_samples_are_in_normalization_range()
  {
  dab::sample_queue_t queue{};
  dab::rtl_file device{queue, kEvenSampleFileName};
  device.run();

  dab::__internal_common::sample_t sample{};
  bool outOfRange{};
  std::size_t size{};

  while(queue.try_dequeue(sample))
    {
    if((sample.imag() > 0.0f && sample.imag() > 1.0f) ||
       (sample.imag() < 0.0f && sample.imag() < -1.0f) ||
       (sample.real() > 0.0f && sample.real() > 1.0f) ||
       (sample.real() < 0.0f && sample.real() < -1.0f))
      {
      outOfRange = true;
      continue;
      }

    ++size;
    }

  ASSERT(!outOfRange);
  ASSERT_EQUAL(4, size);
  }


int main(int argc, char * * argv)
  {
  auto testSuite = cute::suite{};

  testSuite += cute::test{test_enable_invalid_option, "Enabling an invalid option should return false"};
  testSuite += cute::test{test_enable_valid_option, "Enabling a valid option should return true"};
  testSuite += cute::test{test_disable_invalid_option, "Disabling an invalid option should return false"};
  testSuite += cute::test{test_disable_valid_option, "Disabling a valid option should return true"};
  testSuite += cute::test{test_looping_is_disabled_by_default, "Looping should be disabled by default"};
  testSuite += cute::test{test_looping, "Enabling looping should cause looping"};
  testSuite += cute::test{test_non_looping_even_4_samples, "Not looping over the even samples should result in 4 samples"};
  testSuite += cute::test{test_non_looping_odd_4_samples, "Not looping over the odd samples should result in 4 samples"};
  testSuite += cute::test{test_looping_more_samples, "Looping over the even samples should result in more than 4 samples"};
  testSuite += cute::test{test_samples_are_in_normalization_range, "The sample components should be in range [-1,1]"};

  std::random_device device{};
  std::mt19937 generator{device()};
  std::shuffle(testSuite.begin(), testSuite.end(), generator);

  auto xmlFile = cute::xml_file_opener{argc, argv};
  auto listener = cute::xml_listener<cute::ide_listener<>>{xmlFile.out};

  setup();
  auto success = !cute::makeRunner(listener, argc, argv)(testSuite, "RTL_FILE device suite");
  teardown();

  return success;
  }

