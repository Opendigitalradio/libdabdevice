# pylint: disable=missing-docstring
from conans import ConanFile


class LibDABDeviceConan(ConanFile):
    name = 'libdabdevice'
    version = '1.0.1'
    description = (
        'The device abstraction subsystem for the ODR DAB data toolkit, that '
        'provides a uniform interface for different sampling devices.'
    )
    settings = None
    options = {
        'demos': [True, False],
        'test': [True, False],
    }
    default_options = (
        'demos=False',
        'test=True',
    )
    url = 'https://github.com/Opendigitalradio/libdabdevice.git'
    license = 'BSD 3-clause'
    exports_sources = (
        'CMakeLists.txt',
        'LICENSE',
        'README.md',
        'cmake/*',
        'include/*',
        'src/*',
        'test/*',
    )

    def build(self):
        dabdevice_test = '-DDABDEVICE_ENABLE_TESTS=%s' % (
            'On' if self.options.test
            else 'Off'
        )

        dabdevice_demos = '-DDABDEVICE_ENABLE_DEMOS=%s' % (
            'On' if self.options.demos
            else 'Off'
        )

        dabdevice_prefix = '-DCMAKE_INSTALL_PREFIX="%s"' % (
            self.package_folder
        )

        self.run(('cmake {directory}'
                  ' {prefix}'
                  ' {test}'
                  ' {demos}').format(**{
                      'demos': dabdevice_demos,
                      'directory': self.conanfile_directory,
                      'prefix': dabdevice_prefix,
                      'test': dabdevice_test,
                  }))

        self.run('cmake --build . --target install')

    def package_info(self):
        self.cpp_info.libs = [
            'pthread'
        ]
        self.cpp_info.includedirs = [
            'include'
        ]

    def requirements(self):
        self.requires('libdabcommon/[>=1.0]@Opendigitalradio/stable')
        if self.options.test:
            self.requires('CUTEX/[>=1.0]@fmorgner/stable')
