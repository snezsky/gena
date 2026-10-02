"""Runs the tests of a project built with scripts/build.py."""

import argparse
import os
import subprocess
import sys


PROJECT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# stop on the first UndefinedBehaviorSanitizer finding, so it fails the test like AddressSanitizer does
UBSAN_OPTIONS = "print_stacktrace=1:halt_on_error=1"


def parse_arguments():
    parser = argparse.ArgumentParser(description="Runs the tests of a project built with scripts/build.py.")
    parser.add_argument("--preset", required=True, help="configure preset the project was built with")
    parser.add_argument("--junit", metavar="FILE", help="also write a JUnit XML report")
    return parser.parse_args()


def run(command, env):
    print("++ " + " ".join(command), flush=True)
    result = subprocess.run(command, cwd=PROJECT_DIR, env=env)
    if result.returncode != 0:
        sys.exit(result.returncode)


def main():
    args = parse_arguments()
    if args.preset.startswith("msvc") and "VisualStudioVersion" not in os.environ:
        sys.exit("error: run from the x64 Native Tools Command Prompt for VS (or call vcvars64.bat first)")

    env = os.environ.copy()
    env.setdefault("UBSAN_OPTIONS", UBSAN_OPTIONS)

    command = ["ctest", "--preset", args.preset]
    if args.junit:
        # ctest resolves relative paths against the build folder, make it relative to where the script is called
        command += ["--output-junit", os.path.abspath(args.junit)]
    run(command, env)


if __name__ == "__main__":
    main()
