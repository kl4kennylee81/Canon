import json
import os
import os.path


def handle_residual_files(src_json, cache, tset):
	for path, subdirs, files in os.walk('.' + os.sep + 'json'):
		for filename in files:
			splits = filename.split('.')
			name, ext = splits[0], splits[1]
			if 'ai' == name:
				with open(os.path.join(path, filename)) as filepath:
					src_json.update(json.load(filepath))
					continue
			if 'level' == name or 'animations' == name: # important - we modify template cache!
				with open(os.path.join(path, filename)) as filepath:
					cache[name] = json.load(filepath)
					tset.add(name)
					continue


def get_templates_used(json, acc):
	for key in json.keys():
		if key == 'templateKey':
			acc.add(json[key])
		elif type(json[key]) is dict:
			get_templates_used(json[key], acc)


def combine_template_json(template_names, src_json): # takes in a set of keys into template cache
	final_dict = {}
	superset = set()
	
	for template in template_names:
		if template in template_cache: # playercharmale and female not in there, so we need a check
			superset = superset.union(set(template_cache[template].keys()))
	
	for key in superset: # a defaultdict could do this but i'm not in the mood to import
		final_dict[key] = {}

	for key in superset:
		for template in template_names:
			if template in template_cache:
				if key in template_cache[template]:
					final_dict[key].update(template_cache[template][key])

	level_json.update(final_dict)


def populate_test_levels(src_dict):
	for path, subdirs, files in os.walk('.' + os.sep + 'json' + os.sep + 'testLevels'):
		for filename in files:
			src_dict[filename] = (os.path.join(path, filename))


def populate_template_cache(src_dict):
	for path, subdirs, files in os.walk('.' + os.sep + 'json' + os.sep + 'templates'):
		for filename in files:
			splits = filename.split('.')
			name, ext = splits[0], splits[1]

			if ext == 'json':
				with open(os.path.join(path, filename)) as filepath:
					src_dict[name] = json.load(filepath)


test_levels = {} # name to path
template_cache = {} # name to json
populate_test_levels(test_levels)
populate_template_cache(template_cache)

for level in test_levels.keys():

	# load in mapping from test level file
	with open(test_levels[level]) as filepath:
		level_json = json.load(filepath)
	
	template_set = set()
	get_templates_used(level_json, template_set)
	
	# this merges level.json at the template level
	# this inserts ai.json and animation.json at top level
	handle_residual_files(level_json, template_cache, template_set) 
	combine_template_json(template_set, level_json)

	splits = level.split('.')
	name, ext = splits[0], splits[1]
	with open('.' + os.sep + 'json' + os.sep + 'fullLevels' + os.sep + name + 'Full.' + ext, 'w') as file:
		json.dump(level_json, file, indent=4, sort_keys=True)
