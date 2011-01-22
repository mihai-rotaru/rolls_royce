# !/bin/env/python
#APPNAME = "fish"
#VERSION = "0.1"

top = '.'
out = 'waf-build'

def options( opt ):
    opt.load( "compiler_cxx" )

def configure( cnf ):
    print( "configuring the project in " + cnf.path.abspath())
    cnf.load( "compiler_cxx" )
    try:
        cnf.find_program( "g++" )
        cnf.find_program( "ctags" )
    except:
        pass

def build( bld ):
    print( "building..." )
    print( bld.path.abspath())
    bld.program(
            source       = 'src/main.cpp',
            target       = 'fish',
            features     = [ 'cxxprogram' ],
            includes     = [ './freeglut/include' ],
            #defines      = [ '-W1,subsystem,windows' ],
            lib          = [ 'freeglut','opengl32', 'glu32' ],
            libpath      = [ bld.path.abspath() + '/freeglut/lib' ],
            linkflags    = [ '-W1,subsystem,windows' ],
            cxxflags     = [ '-c', '-g', '-O2', '-Wall' ]
            )
