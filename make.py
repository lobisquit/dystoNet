#!/usr/bin/python
from __future__ import print_function

import argparse
import os
import subprocess
import webbrowser
from pathlib import Path

SRC_DIR = Path('src')
BUILD_DIR = Path('build')
DOC_DIR = Path('doc')
TEST_DIR = Path('test')

PROGRAM_RUN_FILE = BUILD_DIR / Path('program.out')
PROGRAM_TEST_FILE = BUILD_DIR / Path('program_test.out')
DOXYFILE = DOC_DIR / Path('Doxyfile')

def exec_command(command, **env_variables):
	''' Execute command via bash with given environment variables '''
	# convert path to string representation to feed subprocess.call
	if isinstance(command, Path):
		command = str(command)

	print('------> ', command)

	# retrieve system environment variables
	env = dict(os.environ)

	# add those passed to exec via kwargs
	env.update(env_variables)

	return subprocess.call(command,
		shell=True,
		# load all environment variable and add new wanted ones
		env=env)

def clean():
	''' Remove all build artifacts from BUILD_DIR '''
	for build_file in Path(BUILD_DIR).glob('*.o'):
		build_file.unlink()

def single_compile(cpp_file, dependencies):
	''' Compile a cpp source file with its dependencies '''
	# build the proper output file name
	output_file = Path("".join(cpp_file.name.split('.')[:-1]) + '.o')

	# obtain dependencies paths as a list of string for next command
	dep_paths = []
	for dependency in dependencies:
		# add SRC_DIR to path if dependency is only the name of the package,
		# not the actual path to it
		if isinstance(dependency, str):
			dep_paths.append( SRC_DIR / Path(dependency) )
		else:
			dep_paths.append( dependency )

	# launch compilation
	return exec_command('g++ -O3 -c -Wall -std=c++0x {input} -o {output} {includes}'.format(
		input = str(cpp_file),
		output = BUILD_DIR / output_file,
		includes = " ".join(["-I" + str(dep) for dep in dep_paths])))

def build(project, dependencies):
	''' Build a project with its dependencies (default all projects) '''
	# compile dependencies before compiling actual project
	for dependency in dependencies:
		deps_cpp_files = Path(SRC_DIR).glob('{}/*.cpp'.format(dependency))
		for cpp_file in deps_cpp_files:
			single_compile(cpp_file, dependencies)

	# compile separately each cpp file, including dependencies
	project_cpp_files = Path(SRC_DIR).glob('{}/*.cpp'.format(project))
	for cpp_file in project_cpp_files:
		single_compile(cpp_file, dependencies)

	# compile every in a single executable
	return exec_command('g++ build/*.o -o {}'.format(PROGRAM_RUN_FILE))

def run(program=PROGRAM_RUN_FILE):
	''' Execute a program, default to output of build step '''
	return exec_command(str(program))

def test(program=PROGRAM_TEST_FILE):
	''' Execute all tests, default by taking all project as dependencies '''

	# retrieve all projects paths, to include them in compilation deps
	dependencies = [path for path in Path(SRC_DIR).iterdir() if path.is_dir()]

	# retrieve cpp of test files
	test_cpp_files = TEST_DIR.glob('*.cpp')

	for cpp_file in test_cpp_files:
		single_compile(cpp_file, dependencies)

	# compile tests, all in one step
	test_result = exec_command('g++ build/*.o -o {}'.format(program))

	# run tests if everything went ok
	if test_result == 0:
		return exec_command(str(program))

def docs(doxyfile=DOXYFILE):
	''' Create doxygen output '''
	return exec_command('doxygen {}'.format(str(doxyfile)))

if __name__ == '__main__':
	# setup command line arguments parser
	parser = argparse.ArgumentParser(description='Manage dystoNet project execution')

	parser.add_argument('--mode',
		dest='mode',
		choices=['build', 'run', 'docs', 'test'],
		default='run',
		help='Select operation mode (voices of this Makefile)')

	parser.add_argument('--project',
		dest='project',
		default='first_coefficients',
		help='Project to compile')

	parser.add_argument('--deps',
		dest='deps',
		default=['utils'],
		nargs='*',
		help='Packets that are needed to compile wanted project')

	cmd_args = parser.parse_args()

	# act accordingly to selected parameters
	if cmd_args.mode in ['build', 'run']:
		clean()
		build_result = build(
			project=cmd_args.project,
			dependencies=cmd_args.deps)

		# run only if build was successfull
		if build_result == 0 and cmd_args.mode == 'run':
			run()

	if cmd_args.mode == 'test':
		clean()
		test()

	if cmd_args.mode == 'docs':
		docs_result = docs()
		# if docs are generated successfully,
		# open index of generated html
		if docs_result == 0:
			index_html_path = Path('doc/html/index.html')
			webbrowser.open_new_tab(str(index_html_path))
