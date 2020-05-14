# Loop through all the subdirectories, and the subdirectories name as default build target name

import os

root_dir = "."
for dir in os.listdir(os.path.join(root_dir, "example")):
    prog_name = dir
    SConscript(os.path.join(root_dir, 'example', prog_name, 'SConscript'), exports=['prog_name'])

