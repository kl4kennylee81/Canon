#!/usr/bin/env python

from subprocess import call

'''
Make sure to call the setup before building
in xcode
'''
call(["python", "full_level_gen.py"])
call(["python", "json_gen.py"])