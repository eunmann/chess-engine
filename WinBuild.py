import glob
from os import path
import subprocess

compiler = 'g++'
warnings = '-Wall -Wextra'
flags = '-std=c++17 -fopenmp'
opt = '-O3 -march=native -flto'
include = '-Iinclude'
out_name = 'chess_engine.exe'
links = '-lpthread'

src_paths = glob.glob(path.join('.', 'src', '*.cpp'))
obj_paths = []

for src_path in src_paths:
    src_path = src_path.replace('src', path.join('bin', 'win', 'obj'))
    src_path = src_path.replace('.cpp', '.o')
    obj_paths.append(src_path)

for i in range(0, len(src_paths)):
    src_path = src_paths[i]
    obj_path = obj_paths[i]

    command = compiler
    command += ' '
    command += warnings
    command += ' '
    command += flags
    command += ' '
    command += opt
    command += ' '
    command += include
    command += ' -c -o'
    command += obj_path
    command += ' '
    command += src_path
    command += ' '
    command += links
    print(command)
    subprocess.call(command, shell=True)

command = compiler
command += ' '
command += warnings
command += ' '
command += flags
command += ' '
command += opt
command += ' '
command += include
command += ' -o '
command += out_name
command += ' '

for obj_path in obj_paths:
    command += obj_path
    command += ' '

command += links

print(command)
subprocess.call(command, shell=True)
