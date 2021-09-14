# -*- coding: utf-8 -*-

from __future__ import print_function

import sys

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


setup(
    name="QEDCascPy",
    description="A package to model strong field QED interactions.",
    author="Robbie Watt",
    license="MIT",
    packages=find_packages(where = 'Source/Python'),
    package_dir={"": "Source/Python"},
    cmake_args=["-DBUILD_PYTHON=ON"],
    cmake_install_dir="Source/Python/QEDCascPy",
    include_package_data = True
)
