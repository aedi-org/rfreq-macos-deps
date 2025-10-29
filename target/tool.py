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
from aedi.utility import apply_unified_diff


class DfuUtilTarget(base.ConfigureMakeDependencyTarget):
    def __init__(self):
        super().__init__('dfu-util')

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://dfu-util.sourceforge.net/releases/dfu-util-0.11.tar.gz',
            'b4b53ba21a82ef7e3d4c47df2952adf5fa494f499b6b0b57c58c5d04ae8ff19e')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('src/dfu_util.h')

    def configure(self, state: BuildState):
        arguments = state.arguments

        if arguments.static_usb:
            # Workaround for missing frameworks pulled by usb
            state.options['LDFLAGS'] += state.run_pkg_config('--libs', 'libusb-1.0')

        if arguments.dfu_util_speedup:
            apply_unified_diff(state.patch_path / 'dfu-util-speedup.diff', state.source)

        super().configure(state)


class OrcTarget(base.MesonSharedTarget):
    def __init__(self):
        super().__init__('orc')

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://gstreamer.freedesktop.org/src/orc/orc-0.4.41.tar.xz',
            'cb1bfd4f655289cd39bc04642d597be9de5427623f0861c1fc19c08d98467fa2')

    def detect(self, state: BuildState) -> bool:
        return state.has_source_file('orc/orc.h')

    def configure(self, state: BuildState):
        opts = state.options
        opts['benchmarks'] = 'disabled'
        opts['examples'] = 'disabled'
        opts['orc-test'] = 'disabled'
        opts['tests'] = 'disabled'

        super().configure(state)


class Rtl433Target(base.CMakeDependencyTarget):
    def __init__(self):
        super().__init__('rtl433')

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://github.com/merbanan/rtl_433/archive/refs/tags/25.02.tar.gz',
            '5a409ea10e6d3d7d4aa5ea91d2d6cc92ebb2d730eb229c7b37ade65458223432',
            patches='rtl433-force-version')


class RtlPowerFftwTarget(base.CMakeDependencyTarget):
    def __init__(self):
        super().__init__('rtl_power_fftw')
        self.prerequisites = 'tclap'

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://github.com/AD-Vega/rtl-power-fftw/archive/cee9a22207ea995bd12adbc6bcfbec92521548b1.tar.gz',
            '261789c7ef874449c03ca6ce4b6f7e0772e57b22da9bfde4e51ab1c641395635')


class StlinkTarget(base.CMakeDependencyTarget):
    def __init__(self):
        super().__init__('stlink')

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://github.com/stlink-org/stlink/archive/refs/tags/v1.8.0.tar.gz',
            'cff760b5c212c2cc480f705b9ca7f3828d6b9c267950c6a547002cd0a1f5f6ac',
            # Build fix patch from https://github.com/stlink-org/stlink/pull/1373/commits
            patches=('stlink-fix-build', 'stlink-relative-chips'))

    def configure(self, state: BuildState):
        if state.arguments.static_usb:
            # Workaround for missing frameworks pulled by usb
            state.options['CMAKE_SHARED_LINKER_FLAGS'] += state.run_pkg_config('--libs', 'libusb-1.0')

        super().configure(state)
