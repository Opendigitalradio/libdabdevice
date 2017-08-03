# pylint: disable=missing-docstring
from conans import ConanFile


class LibDABDeviceConan(ConanFile):
    name = 'libdabdevice'
    description = "The device abstraction layer of the ODR DAB data toolkit"
    license = 'BSD 3-clause'
    version = '1.0.1'
    url = 'https://github.com/Opendigitalradio/libdabdevice.git'
    settings = None
    options = {"test": [True, False]}
    default_options = "test=False"
    exports_sources = ('*', '!.git/*', '!build/*')

    def build(self):
        test = self.options.test

        args = ['-DDABDEVICE_ENABLE_TESTS=%s' % ('On' if test else 'Off')]
        self.run('cmake %s %s %s' % (
            self.conanfile_directory,
            '-DCMAKE_INSTALL_PREFIX="%s"' % self.package_folder,
            ' '.join(args)
        ))
        self.run('cmake --build . --target install')

    def requirements(self):
        if self.options.test:
            self.requires('CUTEX/[>=1.0]@fmorgner/stable')
        self.requires('libdabcommon/[>=1.0]@fmorgner/stable')
