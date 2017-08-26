#!/usr/bin/python
from __future__ import print_function

import argparse
import configparser
import os
import subprocess
import sys
import webbrowser
from collections import OrderedDict
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
		includes = " ".join( ["-I" + str(dep) for dep in dependencies] )))

def merge_compile(output_dir):
	''' Merge all object files in a single executable '''

	# retrieve everything has been compiled in this run
	object_files = [str(obj_file) for obj_file in BUILD_DIR.glob('**/*.o')]

	return exec_command('g++ {} -o {}'.format(
		" ".join(object_files),
		output_dir / PROGRAM_FILE))

def clean():
	''' Remove all build artifacts from BUILD_DIR '''

	for build_file in Path(BUILD_DIR).glob('**/*.o'):
		build_file.unlink()

def build(package, dependencies):
	'''
		Build a package with its dependencies

		Parameters
		----------
		package : Path
			path to the directory containing cpp files to compile
		dependencies : list of Path
			paths of all directories to include in compilation
	'''
	# compile package deps
	for dep in dependencies:
		for cpp_file in dep.glob('*.cpp'):
			# avoid main file of dependencies
			if not cpp_file.name == 'main.cpp':
				# compile including all dependencies, to be sure
				result = single_compile(cpp_file, dependencies=dependencies)

	# compile package files
	if package.parent == TEST_DIR:
		dependencies += [Path('test/'), SRC_DIR / package.stem]

	for cpp_file in package.glob('*.cpp'):
		single_compile(
			cpp_file=cpp_file,
			# include each dependency
			dependencies=dependencies)

	return merge_compile(output_dir=Path('build') / package)

def run(project, config=[]):
	'''
		Execute a program compiled with build

		Parameters
		----------
		project : Path
			path to the directory containing cpp files whose build has to be run
		config: list (default [])
			list containing fields required by script, in the right order and number
	'''
	command = str(Path('build') / project / PROGRAM_FILE)
	return exec_command(command + " " + " ".join(config))

def docs():
	''' Create doxygen output '''
	return exec_command('doxygen {}'.format(DOXYFILE))

if __name__ == '__main__':
	# setup command line arguments parser
	parser = argparse.ArgumentParser(description='Manage dystoNet package execution')

	parser.add_argument('--mode',
		dest='mode',
		choices=['build', 'run', 'docs', 'test'],
		default='run',
		help='Select operation mode (voices of this Makefile)')

	parser.add_argument('--package',
		dest='package',
		default='first_coefficients',
		help='package to compile')

	parser.add_argument('--deps',
		dest='deps',
		default=[],
		nargs='*',
		help='Packets that are needed to compile wanted package')

	parser.add_argument('--configs',
		dest='configs',
		default=['configurations.ini'],
		nargs='+',
		help='Files containing parameter configurations to run')

	cmd_args = parser.parse_args()

	# act accordingly to selected mode parameter
	if cmd_args.mode in ['build', 'run', 'test']:
		# always clean the environment in these cases
		clean()

		# create paths for dependencies in SRC_DIR
		dep_paths = [SRC_DIR / dep for dep in cmd_args.deps]

		if cmd_args.mode in ['build', 'run']:
			# in build mode, package is expected to be in SRC_DIR
			package = SRC_DIR / cmd_args.package

		elif cmd_args.mode in ['test']:
			# in test mode, package is expected to be in TEST_DIR
			package = TEST_DIR / cmd_args.package

		build_result = build(
			package=package,
			dependencies=dep_paths)
		if build_result != 0:
			sys.exit(build_result)

		# run only if build was successfull
		if build_result == 0 and cmd_args.mode in ['test']:
			run_result = run(package)
			if run_result != 0:
				sys.exit(run_result)

		if build_result == 0 and cmd_args.mode in ['run']:
			# read config files given in configs argument
			for config_file in cmd_args.configs:
				config = configparser.ConfigParser(dict_type=OrderedDict)
				config.read(config_file)

				# loop through all section of file
				for section in config.sections():
					print('------> Running section "{}" of "{}"'.format(section, config_file))

					# note that parameters MUST be in the right order in the ini file
					run_result = run(package, config[section].values())
					if run_result != 0:
						sys.exit(run_result)

	if cmd_args.mode == 'docs':
		docs_result = docs()
		# if docs are generated successfully,
		# open index of generated html
		if docs_result == 0:
			index_html_path = Path('doc/html/index.html')
			webbrowser.open_new_tab(str(index_html_path))
		else:
			sys.exit(docs_result)
