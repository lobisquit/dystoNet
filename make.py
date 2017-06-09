#!/usr/bin/python
import argparse
import subprocess
import os
from pathlib import Path

SRC_DIR = Path('src')
BUILD_DIR = Path('build')
PROGRAM_FILE = BUILD_DIR / Path('program.out')

def exec_command(command, output=True, **env_variables):
	''' Execute command via bash with given environment variables '''
	# convert path to string representation to feed subprocess.call
	if isinstance(command, Path):
		command = str(command)

	print('------> ', command)
	return subprocess.call(command,
		shell=True,
		# load all environment variable and add new wanted ones
		env={**env_variables, **dict(os.environ)})

# def list_projects(src_path=SRC_DIR):
# 	return exec_command("cd {} && ls -d */".format(src_path))
#
# def list_files(path):
# 	return exec_command("ls {}*.cpp".format(path))

def clean():
	for build_file in Path(BUILD_DIR).glob('*.o'):
		build_file.unlink()

def single_compile(cpp_file, dependencies):
	# build the proper output file name
	output_file = Path("".join(cpp_file.name.split('.')[:-1]) + '.o')

	# obtain dependencies paths as a list of string for next command
	dep_paths = [str(SRC_DIR / Path(dependency)) for dependency in dependencies]

	# launch compilation
	exec_command('g++ -O3 -c -Wall -std=c++0x {input} -o {output} {includes}'.format(
		input = str(cpp_file),
		output = BUILD_DIR / output_file,
		includes = " ".join(["-I" + dep for dep in dep_paths])))

def build(project='**', dependencies=['utils']):
	# compile dependencies before compiling actual project
	for dependency in dependencies:
		deps_cpp_files = Path(SRC_DIR).glob('{}/*.cpp'.format(dependency))
		for cpp_file in deps_cpp_files:
			print(cpp_file)
			single_compile(cpp_file, dependencies)

	# compile separately each cpp file, including dependencies
	project_cpp_files = Path(SRC_DIR).glob('{}/*.cpp'.format(project))
	for cpp_file in project_cpp_files:
		single_compile(cpp_file, dependencies)

	# compile every in a single executable
	exec_command('g++ build/*.o -o {}'.format(PROGRAM_FILE))

def run(program=PROGRAM_FILE):
	exec_command(str(PROGRAM_FILE), output=False)




clean()
build(project='first_coefficients')
run()


#
# # assess sources tree
# projects = list_projects()
# projects.zip([list])
#
# def build(project):
# 	if not project.endswith('/'):
# 		project += '/'
# 	for source_file in list_files(SRC_DIR + project):
#
# build('utils')
