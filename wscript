# !/bin/env/python
#APPNAME = "fish"
#VERSION = "0.1"

top = '.'
out = 'build'

def options( opt ):
   # opt.load( "compiler_cxx" )
   pass

def configure( cnf ):
    print( "configuring the project in " + cnf.path.abspath())
    cnf.check_tool( "g++" )

def build( bld ):
    print( "building..." )
    a_path = bld.path.abspath()
    print( bld.path.abspath())
    bld.program(
            source       = [ 'main.cpp', 'xmx_Line.cpp' ],
            target       = 'fish',
            features     = [ 'cxxprogram' ],
            includes     = [ './freeglut/include' ],
            lib          = [ 'freeglut','opengl32', 'glu32' ],
            libpath      = [ a_path + '/freeglut/lib', a_path + '/freeglut' ],
            linkflags    = [ '-W1,subsystem,windows' ],
            cxxflags     = [ '-c', '-g', '-O2', '-Wall' ],
            )

# vim:filetype=python
