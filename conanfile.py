# pylint: disable=missing-docstring
from conans import ConanFile, CMake


class LibDABDeviceConan(ConanFile):
    name = 'libdabdevice'
    description = "The device abstraction layer of the ODR DAB data toolkit"
    license = 'BSD 3-clause'
    version = '1.0.1'
    url = 'https://github.com/Opendigitalradio/libdabdevice.git'
    settings = None
    options = {"test": [True, False]}
    default_options = "test=False"
    generators = ['cmake', 'txt']
    exports_sources = (
        'include/*.h'
    )

    def build(self):
        if self.options.test:
            cmake = CMake(self)
            self.run('cmake libdabdevice %s' % cmake.command_line)
            self.run('cmake --build . %s' % cmake.build_config)

    def package(self):
        self.copy('*.h', dst='include', src='include')

    def requirements(self):
        if self.options.test:
            self.requires('CUTEX/[>=1.0]@fmorgner/stable')
        self.requires('libdabcommon/[>=1.0]@fmorgner/stable')
