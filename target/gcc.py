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

import os
import subprocess

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
        opts['--without-zstd'] = None
        super().configure(state)


class ArmNoneEabiGccTarget(base.BuildTarget):
    # TODO: Avoid absolute paths in various files

    def __init__(self):
        super().__init__('arm-none-eabi-gcc')
        self.prerequisites = ('arm-none-eabi-binutils', 'isl', 'mpc')

        # TODO: Add cross-compilation support
        self.multi_platform = False

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://ftpmirror.gnu.org/gcc/gcc-15.1.0/gcc-15.1.0.tar.xz',
            'e2b09ec21660f01fecffb715e0120265216943f038d0e48a9868713e54f06cea')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('gcc/gcc.h')

    def configure(self, state: BuildState):
        super().configure(state)

        args = (
            str(state.source / 'configure'),
            '--prefix=' + self.INSTALL_PREFIX,
            '--build=' + state.host(),
            '--disable-libquadmath',
            '--disable-libssp',
            '--disable-libstdcxx',
            '--disable-multilib',
            '--disable-nls',
            '--disable-shared',
            '--enable-languages=c,c++,lto',
            '--enable-lto',
            '--target=arm-none-eabi',
            '--with-cpu=cortex-m4',
            '--with-fpu=fpv4-sp-d16',
            '--with-newlib',
            '--with-system-zlib',
            '--without-headers',
            '--without-zstd',
        )
        subprocess.run(args, check=True, cwd=state.build_path, env=state.environment)

    def build(self, state: BuildState):
        args = ('make', '--jobs', state.jobs)
        subprocess.run(args, check=True, cwd=state.build_path, env=state.environment)

    def post_build(self, state: BuildState):
        self.install(state)


class ArmNoneEabiNewlibTarget(base.BuildTarget):
    # TODO: Avoid absolute paths in various files

    def __init__(self):
        super().__init__('arm-none-eabi-newlib')

        self.multi_platform = False
        self.prerequisites = ('arm-none-eabi-gcc', 'texinfo')

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://sourceware.org/pub/newlib/newlib-4.5.0.20241231.tar.gz',
            '33f12605e0054965996c25c1382b3e463b0af91799001f5bb8c0630f2ec8c852')

    def configure(self, state: BuildState):
        super().configure(state)

        args = (
            str(state.source / 'configure'),
            '--disable-multilib',
            '--disable-newlib-supplied-syscalls',
            '--disable-nls',
            '--prefix=' + self.INSTALL_PREFIX,
            '--target=arm-none-eabi',
            # Options for newlib-nano
            '--disable-newlib-fseek-optimization',
            '--disable-newlib-fvwrite-in-streamio',
            '--disable-newlib-unbuf-stream-opt',
            '--disable-newlib-wide-orient',
            '--enable-lite-exit',
            '--enable-newlib-global-atexit',
            '--enable-newlib-nano-formatted-io',
            '--enable-newlib-nano-malloc',
            '--enable-newlib-reent-small',
            'CFLAGS_FOR_TARGET=-g -Os -ffunction-sections -fdata-sections -fshort-wchar'
            ' -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16',
        )
        subprocess.run(args, check=True, cwd=state.build_path, env=state.environment)

    def build(self, state: BuildState):
        args = ('make', '--jobs', state.jobs)
        subprocess.run(args, check=True, cwd=state.build_path, env=state.environment)

    def post_build(self, state: BuildState):
        self.install(state)

        # Append suffix to library names manually to match with lib/nano.specs
        lib_path = state.install_path / 'arm-none-eabi/lib'
        lib_suffixes = ('c', 'g', 'rdimon')

        for suffix in lib_suffixes:
            old_path = f'{lib_path}/lib{suffix}.a'
            new_path = f'{lib_path}/lib{suffix}_nano.a'
            os.rename(old_path, new_path)


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
        # Static linking requires PIC, see https://github.com/Homebrew/homebrew-core/issues/19407
        state.options['--with-pic'] = None
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


class TexinfoTarget(base.ConfigureMakeDependencyTarget):
    def __init__(self):
        super().__init__('texinfo')

        # TODO: Add cross-compilation support
        self.multi_platform = False

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://ftpmirror.gnu.org/texinfo/texinfo-7.2.tar.xz',
            '0329d7788fbef113fa82cb80889ca197a344ce0df7646fe000974c5d714363a6')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('man/texinfo.5')
