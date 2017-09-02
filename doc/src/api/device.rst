****************
Device Interface
****************

``#include <dab/device/device.h>``

The core responsibility of **DABDevice** is to provide a common interface for
many different devices, be it real hardware or fake devices based on files. This
section describes the generic interface implemented by all devices.

.. doxygenstruct:: dab::device

Available Devices
=================

As of |version|, **DABDevice** provides implementations for the following
devices:

+--------------------+---------------------+-----------------+------------------+
| **Device type**    | **C++ type**        | **Description** | **Depencencies** |
+====================+=====================+=================+==================+
| RTL SDR USB sticks | ``dab::rtl_device`` | RTL SDR based   | librtlsdr        |
|                    |                     | USB sticks      |                  |
+--------------------+---------------------+-----------------+------------------+
| RTL SDR Raw dumps  | ``dab::rtl_file``   | Dumps acquired  | none             |
|                    |                     | using the       |                  |
|                    |                     | *rtl_sdr*       |                  |
|                    |                     | utility or      |                  |
|                    |                     | similar         |                  |
+--------------------+---------------------+-----------------+------------------+

Device Options
==============

This section introduces the concept of device options and presents the currently
available option keys. It also presents the interface for enabling and disabling
options on a concrete device.

.. _device-option-keys:

Device Option Keys
------------------

.. doxygenenum:: dab::device::option

Device Option Interface
-----------------------

.. doxygenfunction:: dab::device::disable
.. doxygenfunction:: dab::device::enable

Device Descriptors
==================

This section describes the type used for device descriptors. Device descriptors
provide a semi-type-agnostic way of iterating the available devices at runtime.

.. doxygenstruct:: dab::device::descriptor

Descriptor Fields
-----------------

.. doxygenvariable:: dab::device::descriptor::id
.. doxygenvariable:: dab::device::descriptor::kind
.. doxygenvariable:: dab::device::descriptor::manufacturer
.. doxygenvariable:: dab::device::descriptor::serial
.. doxygenvariable:: dab::device::descriptor::type

Device Configuration
====================

This section presents the API available to configure a device. The APIs provide
access to the center frequency and gain of any given device. Please note, that
these APIs don't necessarily apply to software devices.

Device Tuning
-------------

.. doxygenfunction:: dab::device::tune()

Device Gain
-----------

.. doxygenfunction:: dab::device::gain(gain)
.. doxygenfunction:: dab::device::gain() const = 0
.. doxygenfunction:: dab::device::gains

Sample Acquisition
==================

Controlling Sample Acquisition
------------------------------

.. doxygenfunction:: dab::device::run
.. doxygenfunction:: dab::device::stop

Getting Status Information
--------------------------

.. doxygenfunction:: dab::device::running

Non-Members
===========

The functions described in this section are not part of the ``dab::device``
class, but are related to working with devices. In addition to the functions,
there exists the following type alias:

.. doxygentypedef:: dab::device_ptr

.. doxygenfunction:: dab::descriptors
.. doxygenfunction:: dab::make_device
