# !/bin/env/python
#APPNAME = "fish"
#VERSION = "0.1"

from subprocess import *
import re

top = '.'
out = 'build'

def options( opt ):
   # opt.load( "compiler_cxx" )
   pass

def configure( cnf ):
    print( "configuring the project in " + cnf.path.abspath())
    cnf.check_tool( "g++" )
    cnf.find_program( "git" )
    cnf.find_program( "date" )

def build( bld ):
    # determine version
    if bld.env[ 'GIT' ] !="":
        ver = Popen( "git describe" , stdout=PIPE, stderr=PIPE ).stdout.read()
        #short_ver = re.sub('*?
        exe_name = 'fish_'+ver.strip()
        print "building " + str(bld.env['app_name']) + ver.strip() + "..."

    a_path = bld.path.abspath()
    print( bld.path.abspath())
    bld.program(
            source       = [ 'main.cpp', 'xmx_Line.cpp','globals.cpp' ],
            target       = 'fish',
            features     = [ 'cxxprogram' ],
            includes     = [ './freeglut/include' ],
            lib          = [ 'freeglut','opengl32', 'glu32' ],
            libpath      = [ a_path + '/freeglut/lib', a_path + '/freeglut' ],
            linkflags    = [ '-W1,subsystem,windows' ],
            cxxflags     = [ '-c', '-g', '-O2', '-Wall' ],
            )

# vim:filetype=python
