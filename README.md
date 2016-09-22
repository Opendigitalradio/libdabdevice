libdabdevice
===========

**libdabdevice** is a simple Hardware Abstraction Layer (HAL) that aims to iron
out API inconsistencies of different hardware/software devices and provide a
uniform API. This requires you to link with your specific device library that
should be provided by your vendor or a project like osmocom.

**libdabdevice** can be used to acquire complex floating-point baseband samples
from different SDR devices or software "fakes" (like files). These samples are
put into a queue to be consumed by other subsytems like **libdabdemod**.

Code example:
-------------

```cpp
#include <device/rtl_device.h>
#include <types/common_types.h>

#include <future>

int main(int argc, char * * argv)
  {
  using namespace dab::literals;

  dab::sample_queue_t samples{};

  dab::rtl_device device{samples};
  device.enable(dab::device::option::automatic_gain_control);
  device.tune(218640_kHz);

  auto deviceRunner = std::async(std::launch::async, [&]{ device.run(); });

  /*
   * Asynchronously process the received samples
   */
  }
```

Building:
---------

Contrary to the other libraries, **libdabdevice** is designed to be a
header-only library, meaning you do not need to compile it. However, you need
to make sure you link with the correct device library (e.g. librtlsdr) since
**libdabdevice** only abstracts away the API inconsistencies of different devices.

You first need to check out the source code and the respective submodules:

```
$ git clone --recursive https://github.com/Opendigitalradio/libdabdevice.git
```

Like the other members of the **libdabXYZ** family, **libdabdevice** uses CMake
as it build environment. There are several configuration option that
you can choose from when building **libdabdevice**:

| Option                         | Default | Comment                                                 |
| ------------------------------ | ------- | ------------------------------------------------------- |
| `BUILD_DOCUMENTATION`          | **OFF**     | Build the **libdabdevice** documentation.                    |
| `BUILD_DOCUMENTATION_ONLY`     | **OFF**     | Only build the documentation.                           |
| `BUILD_INTERNAL_DOCUMENTATION` | **OFF**     | Generate the developer documentation.                   |
| `CMAKE_BUILD_TYPE`             | **Debug**   | The type of binary to produce.                          |
| `DOCUMENTATION_FOR_THESIS`     | **OFF**     | Build the documentation for the inclusion in the thesis |
| `WITH_ADDRESS_SANITIZER`       | **OFF**     | Include additional memory checks (**slow**)                 |
| `WITH_COMMON_TESTS`            | **OFF**     | Build and run the common library tests.                 |
| `WITH_UNDEFINED_SANITIZER`     | **OFF**     | Check for undefined behavior.                           |


You can use this library as a subproject in any CMake based project. A variable
called `DABDEVICE_INCLUDES` is exported to parent projects, containing the
path to the library headers.
