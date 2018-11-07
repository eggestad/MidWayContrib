#!/usr/bin/env python

from distutils.core import setup, Extension

setup(name="midway",
      version="1.0",
      description="Python MidWay binding",
      author="Terje Eggestad",
      author_email="terje.eggestad@iname.com",
      url="http://www.mid-way.org",
      ext_modules=[Extension("midway", ["midway.c"], libraries=["MidWay"])]
      )
