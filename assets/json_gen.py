import json
import os
import os.path

def get_asset_mapping():

  image_files = {}
  font_files = {}

  abspath = os.path.abspath(__file__)
  dname = os.path.dirname(abspath)

  # get all files
  for path, subdirs, files in os.walk(dname):
    for filename in files:
      splitPath = path.split(os.sep)
      newPathLi = []
      # reverse split path
      for entry in splitPath[::-1]:
        if entry == "assets":
          break
        newPathLi.append(entry)
      newPathLi.append(".")
      newPathLi = newPathLi[::-1]
      newPath = os.sep.join(newPathLi)

      splits = filename.split('.')
      if len(splits) > 1:
        name, ext = splits[0], splits[1]
      else:
        # print filename
        continue
      if ext == 'png':
        image_files[name] = os.path.join(newPath, filename)
      if ext == 'ttf':
        font_files[name] = os.path.join(newPath, filename)

  {i:i for i in range(1, 11)}
  image_mapping = {key : {"file": image_files[key], "minfilter" : "nearest", "magfilter" : "linear", "wrapS" : "clamp", "wrapT" : "clamp"} for key in image_files.keys()}
  font_mapping = {key : {"file": font_files[key], "size": 64} for key in font_files.keys()}

  mapping = {"textures": image_mapping, "fonts": font_mapping}
  return mapping


def modify_json(src_json, file_path_mapping):

  filename = ''
  if 'MenuScreenData' in src_json.keys():
    for name in src_json['MenuScreenData']:
      filename = name   

    if 'UIData' in src_json.keys():
      for menu_name in src_json['UIData']:
        uidata = src_json['UIData'][menu_name]

        target = ''
        if 'textureKey' in uidata.keys():
          target = 'textureKey'
        elif 'uiBackgroundKey' in uidata.keys():
          target = 'uiBackgroundKey' 

        # bonus feature LOL make sure key field is correct
        src_json['UIData'][menu_name]['key'] = menu_name

        # make sure appropriate keys exist
        if target != '':

          # populate mapping of file_path_mapping
          file_path_mapping[src_json['UIData'][menu_name][target]] = filename

          # detect if rename is necessary
          if len(src_json['UIData'][menu_name][target]) > len(filename):
            # condition reads: rename already happened
            if filename + "_" == src_json['UIData'][menu_name][target][:len(filename)+1]:
              continue

          append_name = filename + "_" + src_json['UIData'][menu_name][target]

          # update actual menu json
          src_json['UIData'][menu_name][target] = append_name


def json_files():
  abspath = os.path.abspath(__file__)
  dname = os.path.dirname(abspath)
  for path, subdirs, files in os.walk(dname + os.sep + 'json' + os.sep + 'menus'):
    for filename in files:
      splits = filename.split('.')
      name, ext = splits[0], splits[1]
      if ext == 'json':
        yield os.path.join(path, filename)


def prepend_level_name(texture_map, file_path_mapping):
  # run thru mapping
  for image_name in texture_map.keys():
    if image_name in file_path_mapping.keys():

      # renaming business
      filepath = texture_map[image_name]['file']

      # avoid files that have already been renamed
      splits = filepath.split(os.sep)
      folder = os.sep.join(splits[:-1])
      filename, ext = splits[-1].split('.')

      if len(filename) > len(file_path_mapping[image_name]):
        if file_path_mapping[image_name] + "_" == filename[:len(file_path_mapping[image_name])+1]:
          continue

      new_file = file_path_mapping[image_name] + "_" + filename + '.' + ext

      if not os.path.exists(folder + os.sep + new_file):
        os.rename(filepath, folder + os.sep + new_file)


# ACTUAL MAIN FUNCTION
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)

# step one: make assets mapping
mapping = get_asset_mapping()
with open(dname + os.sep +'json' + os.sep + 'assets.json', 'w+') as file:
  json.dump(mapping, file, indent=4, sort_keys=True)

# step two: get relevant json files
json_file_generator = json_files()

for json_file in json_file_generator:

  json_text = ''
  with open(json_file) as filepath:
    json_text = json.load(filepath)
    # step three: modify file names in json as needed
    file_path_mapping = {}
    modify_json(json_text, file_path_mapping)

    # step four: rename image files
    prepend_level_name(mapping['textures'], file_path_mapping)

    with open(json_file, "w") as filepath:
      json.dump(json_text, filepath, indent=4, sort_keys=True)

# step five: write updated mapping to file
mapping = get_asset_mapping()

# write to assets.json file if json/ directory exists

with open(dname + os.sep +'json' + os.sep + 'assets.json', 'w+') as file:
  json.dump(mapping, file, indent=4, sort_keys=True)

