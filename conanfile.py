from conans import ConanFile

class VoxelNetworkServer(ConanFile):
    name = "VoxelNetwork server"
    version = "1.0"
    license = "Properitary"
    description = "Server code"

    requires = (
        "spdlog/1.8.0",
        "boost/1.75.0",
        "opencl-headers/2020.06.16"
    )

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "*", "!*build/*"

    def build(self):
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy('*.so*', dst='lib', src='lib')
