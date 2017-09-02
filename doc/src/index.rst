Welcome to DABDevice's documentation!
=====================================

.. image:: https://img.shields.io/badge/C%2B%2B-11-green.svg
  :target: https://isocpp.org

.. image:: https://api.bintray.com/packages/fmorgner/conan-fmorgner/libdabdevice%3AOpendigitalradio/images/download.svg
  :target: https://bintray.com/fmorgner/conan-fmorgner/libdabdevice%3AOpendigitalradio/_latestVersion

.. image:: https://img.shields.io/github/issues/Opendigitalradio/libdabdevice.svg
  :target: https://github.com/Opendigitalradio/libdabdevice/issues


DABDevice is a simple hardware abstraction layer (HAL) for SDR devices. It
facilitates uniform access to different kinds of devices, ranging from real
hardware, like RTL-SDR sticks, to fake devices, like raw dump files. DABDevice
is designed to be used in conjuction with other ODR DAB libraries, like
DABDemod, DABDecode and DABIP.

DABDevice is available via
`GitHub <https://github.com/Opendigitalradio/libdabdevice.git>`_ or
`Conan.io <https://https://bintray.com/fmorgner/conan-fmorgner>`_.

The documentation is split into two parts. The :ref:`user-documentation`
describes how to work with DABDevice from a user perspective. It introduces the
user-visible concepts of DABDevice and how to use them. On the other hand,
the :ref:`developer-documentation` describes the internal architecture of
DABDevice and how to write new device abstractions.

Table of contents
------------------

.. toctree::
   :maxdepth: 3

   api/index
   dev/index
