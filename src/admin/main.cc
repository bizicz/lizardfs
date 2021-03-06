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

#include "common/platform.h"

#include <algorithm>
#include <iostream>

#include "admin/chunk_health_command.h"
#include "admin/info_command.h"
#include "admin/io_limits_status_command.h"
#include "admin/list_chunkservers_command.h"
#include "admin/list_disks_command.h"
#include "admin/list_goals_command.h"
#include "admin/list_metadataservers_command.h"
#include "admin/list_mounts_command.h"
#include "admin/list_tapeservers_command.h"
#include "admin/magic_recalculate_metadata_checksum_command.h"
#include "admin/metadataserver_status_command.h"
#include "admin/promote_shadow_command.h"
#include "admin/ready_chunkservers_count_command.h"
#include "admin/reload_config_command.h"
#include "admin/save_metadata_command.h"
#include "admin/stop_master_without_saving_metadata.h"
#include "common/human_readable_format.h"
#include "common/MFSCommunication.h"
#include "common/mfserr.h"

int main(int argc, const char** argv) {
	strerr_init();
	std::vector<const LizardFsProbeCommand*> allCommands = {
			new ChunksHealthCommand(),
			new InfoCommand(),
			new IoLimitsStatusCommand(),
			new ListChunkserversCommand(),
			new ListDisksCommand(),
			new ListGoalsCommand(),
			new ListMountsCommand(),
			new ListMetadataserversCommand(),
			new ListTapeserversCommand(),
			new MetadataserverStatusCommand(),
			new ReadyChunkserversCountCommand(),
			new PromoteShadowCommand(),
			new MetadataserverStopWithoutSavingMetadataCommand(),
			new ReloadConfigCommand(),
			new SaveMetadataCommand(),
			new MagicRecalculateMetadataChecksumCommand(),
	};

	try {
		if (argc < 2) {
			throw WrongUsageException("No command name provided");
		}
		std::string commandName(argv[1]);
		std::vector<std::string> arguments(argv + 2, argv + argc);
		for (auto command : allCommands) {
			if (command->name() == commandName) {
				try {
					std::vector<std::string> supportedOptions;
					for (const auto& optionWithDescription : command->supportedOptions()) {
						supportedOptions.push_back(optionWithDescription.first);
					}
					command->run(Options(supportedOptions, arguments));
					strerr_term();
					return 0;
				} catch (Options::ParseError& ex) {
					throw WrongUsageException("Wrong usage of " + command->name()
							+ "; " + ex.what());
				}
			}
		}
		throw WrongUsageException("Unknown command " + commandName);
	} catch (WrongUsageException& ex) {
		std::cerr << ex.message() << std::endl;
		std::cerr << "Usage:\n";
		std::cerr << "    " << argv[0] << " COMMAND [OPTIONS...] [ARGUMENTS...]\n\n";
		std::cerr << "Available COMMANDs:\n\n";
		for (auto command : allCommands) {
			if (command->name().substr(0, 6) == "magic-") {
				// Treat magic-* commands as undocumented
				continue;
			}
			command->usage();
			if (!command->supportedOptions().empty()) {
				std::cerr << "    Possible command-line options:\n";
				for (const auto& optionWithDescription : command->supportedOptions()) {
					std::cerr << "\n    " << optionWithDescription.first << "\n";
					std::cerr << "        " << optionWithDescription.second << "\n";
				}
			}
			std::cerr << std::endl;
		}
		strerr_term();
		return 1;
	} catch (Exception& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		strerr_term();
		return 1;
	}
}
