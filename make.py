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

PROGRAM_FILE = Path('program.out')
DOXYFILE = DOC_DIR / Path('Doxyfile')

def exec_command(command, **env_variables):
	''' Execute command via bash with given environment variables '''
	# convert path to string representation to feed subprocess.call
	if isinstance(command, Path):
		command = str(command)

	# retrieve system environment variables and add those passed to exec via kwargs
	env = dict(os.environ)
	env.update(env_variables)

	print('------> ', command)
	return subprocess.call(command,
		shell=True,
		# load all environment variable and add new wanted ones
		env=env)

def clean():
	''' Remove all build artifacts from BUILD_DIR '''
	for build_file in Path(BUILD_DIR).glob('**/*.o'):
		build_file.unlink()


def single_compile(cpp_file, dependencies):
	''' Compile a cpp source file with its dependencies '''
	# create the proper output file name
	output_file = BUILD_DIR / cpp_file.parent / (cpp_file.stem + '.o')

	# create a directory for output file, if not present
	output_file.parent.mkdir(parents=True, exist_ok=True)

	# launch compilation
	return exec_command('g++ -O3 -c -Wall -std=c++0x {input} -o {output} {includes}'.format(
		input = cpp_file,
		output = output_file,
		includes = " ".join(["-I" + str(dep) for dep in dependencies])))

def build(project, dependencies):
	'''
		Build a project with its dependencies

		Parameters
		----------
		project : Path
			path to the directory containing cpp files to compile
		dependencies : list of Path
			paths of all directories to include in compilation
	'''
	for dependency in dependencies:
		dep_cpp_files = dependency.glob('**/*.cpp')
		for cpp_file in dep_cpp_files:
			single_compile(cpp_file, dependencies)

	# compile separately each cpp file, including dependencies
	project_cpp_files = project.glob('**/*.cpp')
	for cpp_file in project_cpp_files:
		single_compile(cpp_file, dependencies)

	# retrieve object files of current build
	obj_files = (Path('build') / project.parent ).glob('**/*.o')

	# compile every object file in a single executable
	return exec_command('g++ {} -o {}'.format(
		" ".join( [str(obj_file) for obj_file in obj_files] ),
		Path('build') / project / PROGRAM_FILE))

def run(project):
	'''
		Execute a program compiled with build

		Parameters
		----------
		project : Path
			path to the directory containing cpp files whose build has to be run
	'''
	return exec_command(Path('build') / project / PROGRAM_FILE)

def docs(doxyfile=DOXYFILE):
	''' Create doxygen output '''
	return exec_command('doxygen {}'.format(doxyfile))

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
	if cmd_args.mode in ['build', 'run', 'test']:
		# always clean the environment in these cases
		clean()

		# create paths for dependencies in SRC_DIR
		dep_paths = [SRC_DIR / dep for dep in cmd_args.deps]

		if cmd_args.mode in ['build', 'run']:
			# in build mode, project is expected to be in SRC_DIR
			project = SRC_DIR / cmd_args.project

		elif cmd_args.mode in ['test']:
			# in test mode, project is expected to be in TEST_DIR
			project = TEST_DIR / cmd_args.project

			# add test/ as dependency, since catch.hpp is there
			# and project SRC directory as well
			dep_paths += [Path('test/'), SRC_DIR / cmd_args.project]

		build_result = build(
			project=project,
			dependencies=dep_paths)

		# run only if build was successfull
		if build_result == 0 and cmd_args.mode in ['run', 'test']:
			run(project)

	if cmd_args.mode == 'docs':
		docs_result = docs()
		# if docs are generated successfully,
		# open index of generated html
		if docs_result == 0:
			index_html_path = Path('doc/html/index.html')
			webbrowser.open_new_tab(str(index_html_path))
