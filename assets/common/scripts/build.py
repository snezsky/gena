"""Configures and builds the project with a CMake preset."""

import argparse
import os
import subprocess
import sys


PROJECT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
COVERAGE = "--coverage"
UNIX_SANITIZERS = "-fsanitize=address,undefined -fno-omit-frame-pointer"
MSVC_SANITIZERS = "/fsanitize=address /Zi /D_DISABLE_VECTOR_ANNOTATION /D_DISABLE_STRING_ANNOTATION /D_DISABLE_OPTIONAL_ANNOTATION"


def parse_arguments():
    parser = argparse.ArgumentParser(description="Configures and builds the project with a CMake preset.")
    parser.add_argument("--preset", required=True, help="CMake configure preset (see: cmake --list-presets)")
    parser.add_argument("--coverage", action="store_true", help="build with test coverage (linux only)")
    parser.add_argument("--sanitizers", action="store_true", help="enable ASan (and UBSan on linux)")
    return parser.parse_args()


def compiler_flags(args):
    """Returns the extra compile and link flags for the requested instrumentation."""
    msvc = args.preset.startswith("msvc")
    compile_flags, link_flags = [], []

    if args.sanitizers:
        compile_flags.append(MSVC_SANITIZERS if msvc else UNIX_SANITIZERS)
        if not msvc:
            link_flags.append(UNIX_SANITIZERS)

    if args.coverage:
        link_flags.append(COVERAGE)
        compile_flags.append(COVERAGE)

    return " ".join(compile_flags), " ".join(link_flags)


def run(command, env):
    print("++ " + " ".join(command), flush=True)
    result = subprocess.run(command, cwd=PROJECT_DIR, env=env)
    if result.returncode != 0:
        sys.exit(result.returncode)


def main():
    args = parse_arguments()
    msvc = args.preset.startswith("msvc")
    compile_flags, link_flags = compiler_flags(args)

    if msvc:
        if args.coverage:
            sys.exit("error: --coverage is not supported with MSVC presets")
        if "VisualStudioVersion" not in os.environ:
            sys.exit("error: run from the x64 Native Tools Command Prompt for VS (or call vcvars64.bat first)")

    env = os.environ.copy()
    for name, flags in (("CFLAGS", compile_flags), ("CXXFLAGS", compile_flags), ("LDFLAGS", link_flags)):
        if flags:
            env[name] = f"{env.get(name, '')} {flags}".strip()

    run(["cmake", "--preset", args.preset, "--fresh"], env)
    run(["cmake", "--build", os.path.join("out", "build", args.preset)], env)


if __name__ == "__main__":
    main()
