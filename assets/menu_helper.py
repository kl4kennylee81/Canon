import re
import sys
import os
import json

SKETCH_HEIGHT = 1242
SKETCH_WIDTH = 2209


# command line args
if len(sys.argv) != 2:
	print "i refuse, need an sketch html file"
	sys.exit()

relative_path = sys.argv[1]
if not os.path.exists(relative_path):
	print "your path does not exist"

# read file
data = ''
with open(relative_path) as f:
	data = f.read()

# get parts we want
result = re.search(r'SMApp\((.*)\) \}\);', data)

if not result:
	print "your file doesn't have an SMApp"

# jsonify
json_text = json.loads(result.group(1))
percentages = {}

for layer in json_text['artboards'][0]['layers']:
	layer_dict = {}
	new_id = layer['type'] + layer['name'] # i hope this is unique

	if layer['type'] == 'text':
		layer_dict['fontKey'] = 'menu_body_font' # hard coded
		layer_dict['type'] = 'TEXT'

	# important offset math
	layer_dict['width'] = layer['rect']['width']/float(SKETCH_WIDTH)
	layer_dict['height'] = layer['rect']['height']/float(SKETCH_HEIGHT)

	# sketches uses centered anchor point, but in code we use bottom left anchor
	# layer_dict['x'] = (layer['rect']['x'] - layer['rect']['width']/2)/float(SKETCH_WIDTH)
	# layer_dict['y'] = (layer['rect']['y'] - layer['rect']['height']/2)/float(SKETCH_HEIGHT)

	layer_dict['x'] = layer['rect']['x']/float(SKETCH_WIDTH)
	layer_dict['y'] = 1.0 - layer['rect']['y']/float(SKETCH_HEIGHT) - layer_dict['height']

	layer_dict['key'] = new_id
	percentages[new_id] = layer_dict


# write the percentages dict to a file
with open('tempMenu.json', 'w+') as file:
  json.dump(percentages, file, indent=4, sort_keys=True)