# Copyright 2017 MongoDB Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import SCons

import os
import re
import subprocess

from pkg_resources import parse_version

_icecream_version_min = parse_version("1.1rc2")
_ccache_nocpp2_version = parse_version("3.4.1")


# I'd prefer to use value here, but amazingly, its __str__ returns the
# *initial* value of the Value and not the built value, if
# available. That seems like a bug. In the meantime, make our own very
# sinmple Substition thing.
class _BoundSubstitution:
    def __init__(self, env, expression):
        self.env = env
        self.expression = expression
        self.result = None

    def __str__(self):
        if self.result is None:
            self.result = self.env.subst(self.expression)
        return self.result


def icecc_create_env(env, target, source, for_signature):
    # Safe to assume unix here because icecream only works on Unix
    mkdir = "mkdir -p ${ICECC_VERSION.Dir('').abspath}"

    # Create the env, use awk to get just the tarball name and we store it in
    # the shell variable $ICECC_VERSION_TMP so the subsequent mv command and
    # store it in a known location.
    create_env = "ICECC_VERSION_TMP=$$($ICECC_CREATE_ENV --$ICECC_COMPILER_TYPE $CC $CXX | awk '/^creating .*\\.tar\\.gz/ { print $$2 }')"

    # Simply move our tarball to the expected locale.
    mv = "mv $$ICECC_VERSION_TMP $TARGET"

    # Daisy chain the commands and then let SCons Subst in the rest.
    cmdline = f"{mkdir} && {create_env} && {mv}"
    return cmdline


def generate(env):

    if not exists(env):
        return

    env["ICECCENVCOMSTR"] = env.get("ICECCENVCOMSTR", "Generating environment: $TARGET")
    env["ICECC_COMPILER_TYPE"] = env.get(
        "ICECC_COMPILER_TYPE", os.path.basename(env.WhereIs("${CC}"))
    )
    env.Append(
        BUILDERS={
            "IcecreamEnv": SCons.Builder.Builder(
                action=SCons.Action.CommandGeneratorAction(
                    icecc_create_env, {"comstr": "$ICECCENVCOMSTR"},
                )
            )
        }
    )

    # If we are going to load the ccache tool, but we haven't done so
    # yet, then explicitly do it now. We need the ccache tool to be in
    # place before we setup icecream because we need to do things a
    # little differently if ccache is in play. If you don't use the
    # TOOLS variable to configure your tools, you should explicitly
    # load the ccache tool before you load icecream.
    ccache_enabled = "CCACHE_VERSION" in env
    if "ccache" in env["TOOLS"] and not ccache_enabled:
        env.Tool("ccache")

    # Absoluteify, so we can derive ICERUN
    env["ICECC"] = env.WhereIs("$ICECC")

    if not "ICERUN" in env:
        env["ICERUN"] = env.File("$ICECC").File("icerun")

    # Absoluteify, for parity with ICECC
    env["ICERUN"] = env.WhereIs("$ICERUN")

    env["ICECC_CREATE_ENV"] = env.WhereIs(
        env.get("ICECC_CREATE_ENV", "icecc-create-env")
    )

    # We can't handle sanitizer blacklist files, so disable icecc then, and just flow through
    # icerun to prevent slamming the local system with a huge -j value.
    if any(
        f.startswith("-fsanitize-blacklist=")
        for fs in ["CCFLAGS", "CFLAGS", "CXXFLAGS"]
        for f in env[fs]
    ):
        env["ICECC"] = "$ICERUN"

    # Make CC and CXX absolute paths too. It is better for icecc.
    env["CC"] = env.WhereIs("$CC")
    env["CXX"] = env.WhereIs("$CXX")

    have_explicit_icecc_version = 'ICECC_VERSION' in env and bool(env['ICECC_VERSION'])
    have_icecc_version_url = have_explicit_icecc_version and env["ICECC_VERSION"].startswith("http")

    if have_explicit_icecc_version and not have_icecc_version_url:
        icecc_version_file = env.File('$ICECC_VERSION')
        if not icecc_version_file.exists():
            raise Exception(
                'The ICECC_VERSION variable set set to {}, but this file does not exist'.format(icecc_version_file)
            )
        env['ICECC_VERSION'] = icecc_version_file
    else:
        # Generate the deterministic name for our tarball
        icecc_version_target_filename = env.subst("${CC}${CXX}.tar.gz").replace("/", "_")[
            1:
        ]
        icecc_version_dir = env.Dir("$BUILD_ROOT/scons/icecc")
        icecc_known_version = icecc_version_dir.File(icecc_version_target_filename)

        if have_icecc_version_url:
            # We do the above weaker validation as opposed to
            # urllib.urlparse (or similar). We really only support http
            # URLs here and any other validation either requires a third
            # party module or accepts things we don't.
            env["ICECC_VERSION_URL"] = env["ICECC_VERSION"]
            env["ICECC_VERSION"] = icecc_known_version

            # Use curl / wget to download the toolchain because SCons (and ninja)
            # are better at running shell commands than Python functions.
            curl = env.WhereIs("curl")
            wget = env.WhereIs("wget")

            if curl:
                cmdstr = "curl -L"
            elif wget:
                cmdstr = "wget"
            else:
                raise Exception(
                    "You have specified an ICECC_VERSION that is a URL but you have neither wget nor curl installed."
                )

            env.Command(
                target="$ICECC_VERSION",
                source=["$CC", "$CXX"],
                action=[
                    cmdstr + " -o $TARGET $ICECC_VERSION_URL",
                ],
            )
        else:
            # Make a predictable name for the toolchain
            env["ICECC_VERSION"] = env.File(icecc_known_version)
            env.IcecreamEnv(
                target="$ICECC_VERSION",
                source=["$ICECC_CREATE_ENV", "$CC", "$CXX"],
            )

        # Our ICECC_VERSION isn't just a file, so we need to make
        # things depend on it to ensure that it comes into being at
        # the right time. Don't do that for conftests though: we never
        # want to run them remote.
        def icecc_toolchain_dependency_emitter(target, source, env):
            if "conftest" not in str(target[0]):
                env.Requires(target, "$ICECC_VERSION")
            return target, source

        # Cribbed from Tool/cc.py and Tool/c++.py. It would be better if
        # we could obtain this from SCons.
        _CSuffixes = [".c"]
        if not SCons.Util.case_sensitive_suffixes(".c", ".C"):
            _CSuffixes.append(".C")

        _CXXSuffixes = [".cpp", ".cc", ".cxx", ".c++", ".C++"]
        if SCons.Util.case_sensitive_suffixes(".c", ".C"):
            _CXXSuffixes.append(".C")

        suffixes = _CSuffixes + _CXXSuffixes
        for object_builder in SCons.Tool.createObjBuilders(env):
            emitterdict = object_builder.builder.emitter
            for suffix in emitterdict.keys():
                if not suffix in suffixes:
                    continue
                base = emitterdict[suffix]
                emitterdict[suffix] = SCons.Builder.ListEmitter(
                    [base, icecc_toolchain_dependency_emitter]
                )

    if env.ToolchainIs("clang"):
        env["ENV"]["ICECC_CLANG_REMOTE_CPP"] = 1

        if ccache_enabled and env["CCACHE_VERSION"] >= _ccache_nocpp2_version:
            env.AppendUnique(CCFLAGS=["-frewrite-includes"])
            env["ENV"]["CCACHE_NOCPP2"] = 1
    else:
        env.AppendUnique(CCFLAGS=["-fdirectives-only"])
        if ccache_enabled:
            env["ENV"]["CCACHE_NOCPP2"] = 1

    if "ICECC_SCHEDULER" in env:
        env["ENV"]["USE_SCHEDULER"] = env["ICECC_SCHEDULER"]

    # Build up the string we will set in the environment to tell icecream
    # about the compiler package.
    icecc_version_string = "${ICECC_VERSION.abspath}"
    if "ICECC_VERSION_ARCH" in env:
        icecc_version_string = "${ICECC_VERSION_ARCH}:" + icecc_version_string

    # Use our BoundSubstitition class to put ICECC_VERSION into env['ENV'] with
    # substitution in play. This avoids an early subst which can behave
    # strangely.
    env["ENV"]["ICECC_VERSION"] = _BoundSubstitution(env, icecc_version_string)

    # If ccache is in play we actually want the icecc binary in the
    # CCACHE_PREFIX environment variable, not on the command line, per
    # the ccache documentation on compiler wrappers. Otherwise, just
    # put $ICECC on the command line. We wrap it in the magic "don't
    # consider this part of the build signature" sigils in the hope
    # that enabling and disabling icecream won't cause rebuilds. This
    # is unlikely to really work, since above we have maybe changed
    # compiler flags (things like -fdirectives-only), but we still try
    # to do the right thing.
    if ccache_enabled:
        env["ENV"]["CCACHE_PREFIX"] = _BoundSubstitution(env, "$ICECC")
    else:
        # Make a generator to expand to ICECC in the case where we are
        # not a conftest. We never want to run conftests
        # remotely. Ideally, we would do this for the CCACHE_PREFIX
        # case above, but unfortunately if we did we would never
        # actually see the conftests, because the BoundSubst means
        # that we will never have a meaningful `target` variable when
        # we are in ENV. Instead, rely on the ccache.py tool to do
        # it's own filtering out of conftests.
        def icecc_generator(target, source, env, for_signature):
            if "conftest" not in str(target[0]):
                return '$ICECC'
            return ''
        env['ICECC_GENERATOR'] = icecc_generator

        icecc_string = "$( $ICECC_GENERATOR $)"
        env["CCCOM"] = " ".join([icecc_string, env["CCCOM"]])
        env["CXXCOM"] = " ".join([icecc_string, env["CXXCOM"]])
        env["SHCCCOM"] = " ".join([icecc_string, env["SHCCCOM"]])
        env["SHCXXCOM"] = " ".join([icecc_string, env["SHCXXCOM"]])

    # Make common non-compile jobs flow through icerun so we don't
    # kill the local machine. It would be nice to plumb ICERUN in via
    # SPAWN or SHELL but it is too much. You end up running `icerun
    # icecc ...`, and icecream doesn't handle that. We could try to
    # filter and only apply icerun if icecc wasn't present but that
    # seems fragile. If you find your local machine being overrun by
    # jobs, figure out what sort they are and extend this part of the
    # setup.
    icerun_commands = [
        "ARCOM",
        "LINKCOM",
        "PYTHON",
        "SHLINKCOM",
    ]

    for command in icerun_commands:
        if command in env:
            env[command] = " ".join(["$( $ICERUN $)", env[command]])

    # Uncomment these to debug your icecc integration
    # env['ENV']['ICECC_DEBUG'] = 'debug'
    # env['ENV']['ICECC_LOGFILE'] = 'icecc.log'


def exists(env):

    icecc = env.get("ICECC", False)
    if not icecc:
        return False
    icecc = env.WhereIs(icecc)
    if not icecc:
        return False

    pipe = SCons.Action._subproc(
        env,
        SCons.Util.CLVar(icecc) + ["--version"],
        stdin="devnull",
        stderr="devnull",
        stdout=subprocess.PIPE,
    )

    if pipe.wait() != 0:
        return False

    validated = False
    for line in pipe.stdout:
        line = line.decode("utf-8")
        if validated:
            continue  # consume all data
        version_banner = re.search(r"^ICECC ", line)
        if not version_banner:
            continue
        icecc_version = re.split("ICECC (.+)", line)
        if len(icecc_version) < 2:
            continue
        icecc_version = parse_version(icecc_version[1])
        if icecc_version >= _icecream_version_min:
            validated = True

    if validated:
        env['ICECREAM_VERSION'] = icecc_version

    return validated
