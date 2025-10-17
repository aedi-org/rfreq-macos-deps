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


class _BaseQt6Target(base.CMakeStaticDependencyTarget):
    def __init__(self, name=None):
        super().__init__(name)
        self.generator = 'Ninja'

    def configure(self, state):
        opts = state.options
        opts['FEATURE_framework'] = 'NO'
        opts['QT_NO_FEATURE_AUTO_RESET'] = 'YES'

        super().configure(state)


class Qt6BaseTarget(_BaseQt6Target):
    # TODO: Remove absolute paths from various files inside bin, lib, libexec, mkspecs directories

    def __init__(self):
        super().__init__('qt6base')
        self.project_name = 'QtBase'

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://download.qt.io/official_releases/qt/6.10/6.10.0/submodules/qtbase-everywhere-src-6.10.0.tar.xz',
            'ead4623bcb54a32257c5b3e3a5aec6d16ec96f4cda58d2e003f5a0c16f72046d')

    def configure(self, state):
        state.options['FEATURE_relocatable'] = 'YES'
        super().configure(state)


class Qt6SvgTarget(_BaseQt6Target):
    def __init__(self):
        super().__init__('qt6svg')

        self.prerequisites = 'qt6base'
        self.project_name = 'QtSvg'

    def prepare_source(self, state: BuildState):
        state.download_source(
            'https://download.qt.io/official_releases/qt/6.10/6.10.0/submodules/qtsvg-everywhere-src-6.10.0.tar.xz',
            '5ed2c0e04d5e73ff75c2a2ed92db5dc1788ba70f704fc2b71bc21644beda2533')
