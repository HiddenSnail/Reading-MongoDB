/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/keyfile_option_gen.h --output build/opt/mongo/db/keyfile_option_gen.cpp src/mongo/db/keyfile_option.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/keyfile_option_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/util/options_parser/option_section.h"
#include "mongo/util/options_parser/startup_option_init.h"
#include "mongo/util/options_parser/startup_options.h"

namespace mongo {


Status addKeyfileServerOption(optionenvironment::OptionSection* options_ptr) {
    auto& options = *options_ptr;
    namespace moe = ::mongo::optionenvironment;

    {
        moe::OptionSection section("General options");

        section
            .addOptionChaining("security.keyFile", "keyFile", moe::String, "Private key for cluster authentication", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
            .incompatibleWith("noauth")
        ;

        auto status = options.addSection(section);
        if (!status.isOK()) {
            return status;
        }
    }

    return Status::OK();
}

}  // namespace mongo
