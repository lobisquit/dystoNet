#!/usr/bin/python
import argparse
import subprocess
import os
from pathlib import Path
import webbrowser

SRC_DIR = Path('src')
BUILD_DIR = Path('build')
DOC_DIR = Path('doc')

PROGRAM_FILE = BUILD_DIR / Path('program.out')
DOXYFILE = DOC_DIR / Path('Doxyfile')

def exec_command(command, **env_variables):
	''' Execute command via bash with given environment variables '''
	# convert path to string representation to feed subprocess.call
	if isinstance(command, Path):
		command = str(command)

	print('------> ', command)
	return subprocess.call(command,
		shell=True,
		# load all environment variable and add new wanted ones
		env={**env_variables, **dict(os.environ)})

def clean():
	''' Remove all build artifacts from BUILD_DIR '''
	for build_file in Path(BUILD_DIR).glob('*.o'):
		build_file.unlink()

def single_compile(cpp_file, dependencies):
	''' Compile a cpp source file with its dependencies '''
	# build the proper output file name
	output_file = Path("".join(cpp_file.name.split('.')[:-1]) + '.o')

	# obtain dependencies paths as a list of string for next command
	dep_paths = [str(SRC_DIR / Path(dependency)) for dependency in dependencies]

	# launch compilation
	return exec_command('g++ -O3 -c -Wall -std=c++0x {input} -o {output} {includes}'.format(
		input = str(cpp_file),
		output = BUILD_DIR / output_file,
		includes = " ".join(["-I" + dep for dep in dep_paths])))

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
	return exec_command('g++ build/*.o -o {}'.format(PROGRAM_FILE))

def run(program=PROGRAM_FILE):
	''' Execute a program, default to output of build step '''
	return exec_command(str(program))

def docs(doxyfile=DOXYFILE):
	''' Create doxygen output '''
	return exec_command('doxygen {}'.format(str(doxyfile)))

if __name__ == '__main__':
	# setup command line arguments parser
	parser = argparse.ArgumentParser(description='Manage dystoNet project execution')

	parser.add_argument('--mode',
		dest='mode',
		choices=['build', 'run', 'docs'],
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
	if cmd_args.mode == 'build' or cmd_args.mode == 'run':
		clean()
		build_result = build(
			project=cmd_args.project,
			dependencies=cmd_args.deps)

		# run only if build was successfull
		if build_result == 0 and cmd_args.mode == 'run':
			run()

	if cmd_args.mode == 'docs':
		docs_result = docs()
		# if docs are generated successfully,
		# open index of generated html
		if docs_result == 0:
			index_html_path = Path('doc/html/index.html')
			webbrowser.open_new_tab(str(index_html_path))
