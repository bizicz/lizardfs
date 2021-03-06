/*
   Copyright 2013-2014 EditShare, 2013-2015 Skytechnology sp. z o.o.

   This file is part of LizardFS.

   LizardFS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3.

   LizardFS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with LizardFS. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/platform.h"

#include <tuple>

#include "common/chunk_type.h"
#include "common/media_label.h"
#include "common/network_address.h"
#include "common/serialization_macros.h"

SERIALIZABLE_CLASS_BEGIN(ChunkWithAddressAndLabel)
SERIALIZABLE_CLASS_BODY(ChunkWithAddressAndLabel,
		NetworkAddress, address,
		MediaLabel    , label,
		ChunkType     , chunkType)

	bool operator==(const ChunkWithAddressAndLabel& other) const {
		return std::make_tuple(address, label, chunkType)
				== std::make_tuple(other.address, other.label, other.chunkType);
	}

	bool operator<(const ChunkWithAddressAndLabel& other) const {
		return std::make_tuple(address, label, chunkType)
				< std::make_tuple(other.address, other.label, other.chunkType);
	}
SERIALIZABLE_CLASS_END;
