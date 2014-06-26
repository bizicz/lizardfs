#include "config.h"
#include "common/rotate_files.h"

#include <syslog.h>
#include <string>
#include <boost/filesystem.hpp>

#include "common/slogger.h"

namespace {

void rotateFile(bool ifExistsOnly, const std::string& from, const std::string& to) {
	namespace fs = boost::filesystem;
	if (ifExistsOnly) {
		if (!fs::exists(from)) {
			return;
		}
	}
	try {
		fs::rename(from, to);
	} catch (const std::exception& e) {
		mfs_arg_errlog(LOG_ERR, "rename backup file %s to %s failed (%s)", from.c_str(), to.c_str(), e.what());
	}
}

}

void rotateFiles(const std::string& file, int storedPreviousCopies) {
	// rename previous backups
	if (storedPreviousCopies > 0) {
		for (int n = storedPreviousCopies; n > 1; n--) {
			rotateFile(true,
					file + "." + std::to_string(n - 1),
					file + "." + std::to_string(n));
		}
		rotateFile(true, file, file + ".1");
	}
}

void rotateFiles(const std::string& from, const std::string& to, int storedPreviousCopies) {
	rotateFiles(to, storedPreviousCopies);
	rotateFile(false, from, to);
}

