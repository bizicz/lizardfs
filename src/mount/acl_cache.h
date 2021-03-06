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

#include <memory>

#include "common/exception.h"
#include "mount/mastercomm.h"

LIZARDFS_CREATE_EXCEPTION_CLASS_MSG(AclAcquisitionException, Exception, "ACL acquiring");

typedef std::shared_ptr<AccessControlList> AclCacheEntry;

typedef LruCache<
		LruCacheOption::UseTreeMap,
		LruCacheOption::Reentrant,
		AclCacheEntry,
		uint32_t, uint32_t, uint32_t, AclType> AclCache;

inline AclCacheEntry getAcl(uint32_t inode, uint32_t uid, uint32_t gid, AclType type) {
	AclCacheEntry acl(new AccessControlList());
	uint8_t status = fs_getacl(inode, uid, gid, type, *acl);
	if (status == STATUS_OK) {
		return acl;
	} else if (status == ERROR_ENOATTR) {
		return AclCacheEntry();
	} else {
		throw AclAcquisitionException(status);
	}
}
