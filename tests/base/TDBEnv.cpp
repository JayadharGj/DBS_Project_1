#include "base/TDBEnv.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <absl/flags/flag.h>
#include <absl/flags/declare.h>

#include "utils/fsutils.h"

ABSL_FLAG(bool, disable_logs, true,
          "Whether to disable the log printing to stderr");

ABSL_DECLARE_FLAG(std::string, tmpdir);

namespace taco {

bool g_unlink_failure = false;
bool g_unexpected_user_unlink_failure = false;

TDBEnv::~TDBEnv() {}

void
TDBEnv::SetUp() {
    // We always want to override the umask settings in the tests, as we assume
    // the open(2) calls will honor whatever mode we pass to it.
    umask(0);

    if (absl::GetFlag(FLAGS_tmpdir).empty()) {
        if (strlen(LOCAL_DATA_DIR_PREFIX) > 0) {
            // Try to create/access LOCAL_DATA_DIR_PREFIX/$USER/cse562_tmps
            std::string tmpdir = LOCAL_DATA_DIR_PREFIX;
            tmpdir += cuserid(nullptr);
            tmpdir += "/cse562_tmps";

            if (dir_exists(tmpdir.c_str())) {
                // AT_EACCESS flag makes faccessat to perform access checks
                // using effective user and group IDs.
                if (!faccessat(0, tmpdir.c_str(),
                               R_OK | W_OK | X_OK, AT_EACCESS)) {
                    // ok
                    absl::SetFlag(&FLAGS_tmpdir, tmpdir);
                }
            } else {
                // can we create one?
                std::string tmpdir_copy = tmpdir;
                if (!pg_mkdir_p(&tmpdir_copy[0], 0700)) {
                    // yes
                    absl::SetFlag(&FLAGS_tmpdir, tmpdir);
                }
            }
        }

        // fall back to BUILDDIR/tmp
        if (absl::GetFlag(FLAGS_tmpdir).empty()) {
            absl::SetFlag(&FLAGS_tmpdir, BUILDDIR "/tmp");
        }
    }

    try {
        if (absl::GetFlag(FLAGS_disable_logs)) {
            DisableLogPrint();
        }

        // static initialization of the database
        Database::init_global();
    } catch (const TDBError &e) {
        FAIL() << "unexpected error: " << e.GetMessage();
    } catch (...) {
        FAIL() << "unexpected unknown exception";
    }
}

void
TDBEnv::TearDown() {
    // enable all messages at the end so that we can show any warning messages
    // we may need to print
    SetLogPrintMinSeverity(kInfo);

    try {
        g_db->close();
    } catch (const TDBError &e) {
        FAIL() << "unexpected error: " << e.GetMessage();
    } catch (...) {
        FAIL() << "unexpected unknown exception";
    }

    if (g_unlink_failure) {
        LOG(kWarning,
            "some temporary files/directories were not successfully removed");
    }

    if (g_unexpected_user_unlink_failure) {
        LOG(kWarning,
            "some temporary files/directories expected to be removed by the "
            "tests still exist");

    }
}

void
TestEnableLogging() {
    if (absl::GetFlag(FLAGS_disable_logs)) {
        SetLogPrintMinSeverity(kInfo);
    }
}

void
TestDisableLogging() {
    if (absl::GetFlag(FLAGS_disable_logs)) {
        DisableLogPrint();
    }
}

}   // namespace taco
