import os

rootdir = os.curdir + "/test"

for subdir, dirs, files in os.walk(rootdir):
    for file in files:
        if(file.endswith(".cpp")):
            path = os.path.join(subdir, file).replace('\\', '/')
            print(f'"{path}",')
