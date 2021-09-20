# -*- coding: utf-8 -*-

from __future__ import print_function

import sys, os

try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

from setuptools import find_packages
from setuptools.command.install import install

"""
class InstallCommand(install):
    user_options = install.user_options + [
        ('openmp', None, "Build package wih OpenMP."),
    ]

    def initialize_options(self):
        install.initialize_options(self)
        self.openmp = None

    def finalize_options(self):
        install.finalize_options(self)

    def run(self):
        global openmp
        exit()
        openmp = self.openmp # will be 1 or None
        install.run(self)
"""

def set_cmake_args():
    openmp_arg = os.environ.get('BUILD_OPENMP', default=None)
    if openmp_arg:
        return ["-DBUILD_PYTHON=ON", "-DBUILD_OPENMP=" + openmp_arg]
    else:
        return ["-DBUILD_PYTHON=ON"]

setup(
    name="QEDCascPy",
    description="A package to model strong field QED interactions.",
    author="Robbie Watt",
    license="MIT",
    packages=find_packages(where = 'Source/Python'),
    package_dir={"": "Source/Python"},
    cmake_args=set_cmake_args(),
    cmake_install_dir="Source/Python/QEDCascPy",
    include_package_data = True
)
