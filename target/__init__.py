#
#    Module to build radio frequency libraries and tools for macOS
#    Copyright (C) 2020-2026 Alexey Lysiuk
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

from .gcc import *
from .library import *
from .main import *
from .qt import *
from .tool import *


def targets():
    return (
        LibreCalGuiTarget(),
        LibreVnaGuiTarget(),
        SdrPlusPlusTarget(),
        SrdppExpTarget(),

        # Libraries
        Ad9361Target(),
        AirspyTarget(),
        AirspyHFTarget(),
        BladeRFTarget(),
        Codec2Target(),
        CorrectTarget(),
        FftwTarget(),
        FobosTarget(),
        FobosAgileTarget(),
        GlfwTarget(),
        HackRFTarget(),
        HydraSdrTarget(),
        IioTarget(),
        LimeSuiteTarget(),
        MakoTarget(),
        MarkupSafeTarget(),
        PerseusTarget(),
        PortAudioTarget(),
        RfnmAudioTarget(),
        RtAudioTarget(),
        RtlSdrTarget(),
        SDRplayTarget(),
        SpdLogTarget(),
        TclapTarget(),
        UsbTarget(),
        VolkTarget(),
        ZstdTarget(),

        # GCC
        ArmNoneEabiBinutilsTarget(),
        ArmNoneEabiGcc13Target(),
        ArmNoneEabiGcc14Target(),
        ArmNoneEabiGccTarget(),
        ArmNoneEabiGdbTarget(),
        ArmNoneEabiNewlibTarget(),
        GmpTarget(),
        IslTarget(),
        MpcTarget(),
        MpfrTarget(),
        TexinfoTarget(),

        # Qt
        Qt6BaseTarget(),
        Qt6ChartsTarget(),
        Qt6SvgTarget(),

        # Tools
        DfuUtilTarget(),
        OrcTarget(),
        Rtl433Target(),
        RtlPowerFftwTarget(),
        StlinkTarget(),
    )
