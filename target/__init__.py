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

from .library import *
from .main import *
from .tool import *


def targets():
    return (
        # Libraries
        Ad9361Target(),
        FftwTarget(),
        FobosTarget(),
        GlfwTarget(),
        HackRFTarget(),
        IioTarget(),
        MakoTarget(),
        MarkupSafeTarget(),
        RtlSdrTarget(),
        UsbTarget(),
        ZstdTarget(),

        # Tools
        DfuUtilTarget(),
        OrcTarget(),
    )
