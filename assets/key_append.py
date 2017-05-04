import json
import os
import os.path


def add_json_keys(src_json, key_set):
  for key in src_json.keys():

    if key == 'waves':
      for child_key in src_json[key].keys():
        for obj in src_json[key][child_key]['waveEntries'].keys():
          src_json[key][child_key]['waveEntries'][obj]['key'] = obj
    
    elif key == 'levels':
      for child_key in src_json[key].keys():
        for entrylist in src_json[key][child_key].keys():
          for entry in src_json[key][child_key][entrylist].keys():
              src_json[key][child_key][entrylist][entry]['key'] = entry

    elif key in key_set:
      for child_key in src_json[key].keys():
        src_json[key][child_key]['key'] = child_key


def json_files():
  abspath = os.path.abspath(__file__)
  dname = os.path.dirname(abspath)
  for path, subdirs, files in os.walk(dname + os.sep + 'json'):
    for filename in files:
      splits = filename.split('.')
      name, ext = splits[0], splits[1]
      if ext == 'json':
        yield os.path.join(path, filename)


#edge cases: menus, waves, levels
generator = json_files()
keynames = {'templates', 'sounds', 'zones', 'ai', 'UIData', 'animations', 'paths', 'shapes', 'objects', 'shapes'}

for json_file in generator:
  json_text = ''

  with open(json_file) as filepath:
    json_text = json.load(filepath)
    add_json_keys(json_text, keynames)

  with open(json_file, "w") as filepath:
    json.dump(json_text, filepath, indent=4, sort_keys=True)