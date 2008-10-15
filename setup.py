#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Python Bindings for libtextcat
#
# Copyright (c) 2008 Per Øyvind Karlsen <peroyvind@mandriva.org>
#
# All rights reserved.
import sys, os
from warnings import warn
from setuptools import setup, Extension

descr = "Python bindings for libmetalink"
long_descr = """PylibMetalink provides a python interface for the libmetalink
library."""
version = '0.1'
version_define = [('VERSION', '"%s"' % version)]

# FIXME: Probably some nicer way to do this
if 'sdist' in sys.argv:
    os.system('bzr log . > ChangeLog')
c_files = ['libmetalink.c']
compile_args = []
warnflags = ['-Wall', '-Wextra']
compile_args.extend(warnflags)
link_args = ['-lmetalink']
extens=[Extension('cMetalink', c_files, extra_compile_args=compile_args, extra_link_args=link_args, define_macros=version_define)]

setup(
    name = "pylibmetalink",
    version = version,
    description = descr,
    author = "Per Øyvind Karlsen",
    author_email = "peroyvind@mandriva.org",
    url = "http://launchpad.net/pylibmetalink",
    license = 'MIt',
    long_description = long_descr,
    platforms = sys.platform,
    classifiers = [
        'Development Status :: 3 - Alpha',
        'Programming Language :: Python',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Operating System :: POSIX :: Linux'
    ],
    ext_modules = extens,
)

sys.exit(0)
