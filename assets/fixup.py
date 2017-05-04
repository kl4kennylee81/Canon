import json   
json_txt = """
{
"ROLE_NAME": {
	"FOO": {
		"download_url": "http: //something.staging/12345/buzz.zip"
	},
	"BAR": {
		"download_url": "http: //something.staging/12345/fizz.zip"
	},
	"download_url": "http: //something.staging/12345/fizzbuzz.zip",
	"db_name": "somedb",
	"db_server": "dbserver.staging.dmz",
	"plugin": {
		"server_url": "http: //lab.staging.corp/server/"
	}
}
}
"""
data = json.loads(json_txt)
def fixup(adict, k):
	for key in adict.keys():
		if key == k:
			adict[key] = float(v)/32.0
		elif type(adict[key]) is dict:
			fixup(adict[key], k, v)

		elif type(adict[key]) is list: 
			for element in adict[key]:
				fixup(element,k,v)

import pprint
pprint.pprint( data )

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
for path, subdirs, files in os.walk(dname + os.sep + 'json' + os.sep + 'templates'):
	for filename in files:
		splits = filename.split('.')
		name, ext = splits[0], splits[1]

		if ext == 'json':
			with open(os.path.join(path, filename)) as filepath:
				data = json.load(filepath)
				fixup(data, 'x');
				fixup(data, 'y');

pprint.pprint( data )    