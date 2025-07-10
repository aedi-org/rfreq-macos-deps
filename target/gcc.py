#
#    Module to build radio frequency libraries and tools for macOS
#    Copyright (C) 2025 Alexey Lysiuk
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

from aedi.state import BuildState
from aedi.target import base


class ArmNoneEabiBinutilsTarget(base.ConfigureMakeDependencyTarget):
    def __init__(self):
        super().__init__('arm-none-eabi-binutils')

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://ftpmirror.gnu.org/binutils/binutils-2.44.tar.bz2',
            'f66390a661faa117d00fab2e79cf2dc9d097b42cc296bf3f8677d1e7b452dc3a')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('binutils/doc/binutils.info')

    def configure(self, state: BuildState):
        opts = state.options
        opts['--target'] = 'arm-none-eabi'
        opts['--with-system-zlib'] = None
        super().configure(state)


class ArmNoneEabiGccTarget(base.ConfigureMakeDependencyTarget):
    def __init__(self):
        super().__init__('arm-none-eabi-gcc')
        self.prerequisites = ('arm-none-eabi-binutils', 'isl', 'mpc')

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://ftpmirror.gnu.org/gcc/gcc-15.1.0/gcc-15.1.0.tar.xz',
            'e2b09ec21660f01fecffb715e0120265216943f038d0e48a9868713e54f06cea')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('gcc/gcc.h')

    def configure(self, state: BuildState):
        opts = state.options
        opts['--enable-languages'] = 'c,c++,lto'
        opts['--enable-lto'] = None
        opts['--enable-multilib'] = None
        opts['--target'] = 'arm-none-eabi'
        opts['--with-multilib-list'] = 'aprofile,rmprofile'
        opts['--with-system-zlib'] = None

        super().configure(state)


class GmpTarget(base.ConfigureMakeStaticDependencyTarget):
    def __init__(self):
        super().__init__('gmp')

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://gmplib.org/download/gmp/gmp-6.3.0.tar.xz',
            'a3c2b80201b89e68616f4ad30bc66aee4927c3ce50e33929ca819d5c43538898')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('gmp.pc.in')

    def configure(self, state: BuildState):
        opts = state.options
        opts['--enable-cxx'] = None
        opts['--with-pic'] = None  # https://github.com/Homebrew/homebrew-core/issues/19407

        super().configure(state)

    def post_build(self, state: BuildState):
        super().post_build(state)

        replacements = {
            '#define __GMP_CC ': 'clang\n',
            '#define __GMP_CFLAGS ': '\n',
        }

        def cleanup_cc_cflags(line: str):
            for prefix, replacement in replacements.items():
                if line.startswith(prefix):
                    return prefix + replacement

            return line

        self.update_text_file(state.install_path / 'include/gmp.h', cleanup_cc_cflags)


class IslTarget(base.ConfigureMakeStaticDependencyTarget):
    def __init__(self):
        super().__init__('isl')
        self.prerequisites = ('gmp',)

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://libisl.sourceforge.io/isl-0.27.tar.xz',
            '6d8babb59e7b672e8cb7870e874f3f7b813b6e00e6af3f8b04f7579965643d5c')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('isl_int.h')

    def configure(self, state: BuildState):
        state.environment['CXXFLAGS'] = '-std=c++17'  # required to compile isl_test_cpp17
        super().configure(state)


class MpcTarget(base.ConfigureMakeStaticDependencyTarget):
    def __init__(self):
        super().__init__('mpc')
        self.prerequisites = ('mpfr',)

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://ftpmirror.gnu.org/mpc/mpc-1.3.1.tar.gz',
            'ab642492f5cf882b74aa0cb730cd410a81edcdbec895183ce930e706c1c759b8')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('src/mpc.h')


class MpfrTarget(base.ConfigureMakeStaticDependencyTarget):
    def __init__(self):
        super().__init__('mpfr')
        self.prerequisites = ('gmp',)

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://ftpmirror.gnu.org/mpfr/mpfr-4.2.2.tar.xz',
            'b67ba0383ef7e8a8563734e2e889ef5ec3c3b898a01d00fa0a6869ad81c6ce01')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('mpfr.pc.in')
