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
