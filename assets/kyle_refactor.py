import json
import os
import os.path


def populate_test_levels(src_dict):
	abspath = os.path.abspath(__file__)
	dname = os.path.dirname(abspath)
	for path, subdirs, files in os.walk(dname + os.sep + 'json' + os.sep + 'testLevels'):
		for filename in files:
			src_dict[filename] = (os.path.join(path, filename))


def update_wave_entry(entry_json):
	new_json = entry_json

	if(new_json["templateKey"] == "birb" and new_json["aiKey"] != "homing"):
		new_json["templateKey"] = "blobo"
		new_json["zoneKeys"] = ["blobo_zone"]

	if(new_json["templateKey"] == "birb" and new_json["aiKey"] == "homing"):
		new_json["zoneKeys"] = ["birb_zone"]

	if(new_json["templateKey"] == "blobo" and new_json["aiKey"] == "homing"):
		new_json["templateKey"] = "birb"

	return new_json


def update_wave_entries(entries_json):
	new_json = entries_json
	for key in entries_json:
		new_json[key] = update_wave_entry(new_json[key])
	return new_json

def update_wave(wave_json):
	new_json = wave_json
	new_json["waveEntries"] = update_wave_entries(new_json["waveEntries"])
	return new_json

def update_waves(waves_json):
	new_json = waves_json
	for key in waves_json:
		new_json[key] = update_wave(waves_json[key])
	return new_json


test_levels = {} # name to path
template_cache = {} # name to json
populate_test_levels(test_levels)

for level in test_levels.keys():

	# load in mapping from test level file
	with open(test_levels[level]) as filepath:
		level_json = json.load(filepath)

	new_json = update_waves(level_json["waves"])

	splits = level.split('.')
	name, ext = splits[0], splits[1]
	abspath = os.path.abspath(__file__)
	dname = os.path.dirname(abspath)

	fullLevelDir = dname + os.sep + 'json' + os.sep + 'testLevels'
	if not os.path.exists(fullLevelDir):
	    os.makedirs(fullLevelDir)
	with open(fullLevelDir + os.sep + name + '.' + ext, 'w+') as f:
		json.dump(level_json, f, indent=4, sort_keys=True)

