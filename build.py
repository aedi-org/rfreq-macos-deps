#!/usr/bin/env python3

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
import sys

_min_version = (3, 8, 0, 'final', 0)

if sys.version_info < _min_version:
    print(f'This module requires Python {_min_version[0]}.{_min_version[1]}.{_min_version[2]} or newer')
    sys.exit(1)

os.environ['PYTHONDONTWRITEBYTECODE'] = '1'
sys.dont_write_bytecode = True

root_path = os.path.abspath(os.path.dirname(__file__))
sys.path.append(f'{root_path}{os.sep}core')

# pylint: disable=wrong-import-position
import aedi  # noqa: E402

# pylint: disable=wrong-import-position
import target  # noqa: E402


def _main():
    builder = aedi.Builder()
    builder.targets += target.targets()
    builder.run(sys.argv[1:])


if __name__ == '__main__':
    _main()
