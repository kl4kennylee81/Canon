import json
import os
import os.path

image_files = {}
font_files = {}

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)

# get all files
for path, subdirs, files in os.walk(dname):
  for filename in files:
    splitPath = path.split("/")
    newPathLi = []
    # reverse split path
    for entry in splitPath[::-1]:
      if entry == "assets":
        break
      newPathLi.append(entry)
    newPathLi.append(".")
    newPathLi = newPathLi[::-1]
    newPath = "/".join(newPathLi)

    splits = filename.split('.')
    name, ext = splits[0], splits[1]
    if ext == 'png':
      image_files[name] = os.path.join(newPath, filename)
    if ext == 'ttf':
      font_files[name] = os.path.join(newPath, filename)

{i:i for i in range(1, 11)}
image_mapping = {key : {"file": image_files[key], "minfilter" : "nearest", "magfilter" : "linear", "wrapS" : "clamp", "wrapT" : "clamp"} for key in image_files.keys()}
font_mapping = {key : {"file": font_files[key], "size": 64} for key in font_files.keys()}

mapping = {"textures": image_mapping, "fonts": font_mapping}
# data = json.dumps(mapping, sort_keys=False, indent=4)

# write to assets.json file if json/ directory exists
with open(dname + os.sep +'json/assets.json', 'w+') as file:
  json.dump(mapping, file, indent=4, sort_keys=True)