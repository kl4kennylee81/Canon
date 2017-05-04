import json   
import json
import os
import os.path

# json_txt = """
# {
# "ROLE_NAME": {
# 	"FOO": {
# 		"download_url": "http: //something.staging/12345/buzz.zip"
# 	},
# 	"BAR": {
# 		"download_url": "http: //something.staging/12345/fizz.zip"
# 	},
# 	"download_url": "http: //something.staging/12345/fizzbuzz.zip",
# 	"db_name": "somedb",
# 	"db_server": "dbserver.staging.dmz",
# 	"plugin": {
# 		"server_url": "http: //lab.staging.corp/server/"
# 	}
# }
# }
# """

def fixup(adict, k):
	if type(adict) is not dict:
		return;
	for key in adict.keys():
		print(key)
		if type(adict[key]) is dict:
			fixup(adict[key], k);

		elif type(adict[key]) is list: 
				if (key == k):
					newV = []
					for v in adict[key]:
						newV.append(float(v)/32.0)
					adict[key] = newV;
		elif key == k:
			adict[key] = float(adict[key])/32.0

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
for path, subdirs, files in os.walk(dname + os.sep + 'json' + os.sep + 'testLevels'):
	for filename in files:
		splits = filename.split('.')
		name, ext = splits[0], splits[1]

		if ext == 'json':
			with open(os.path.join(path, filename)) as filepath:
				data = json.load(filepath)
				fixup(data, 'x');
				fixup(data, 'y');
				fixup(data, "vertices")
			with open(os.path.join(path, filename),"w") as filepath:
				json.dump(data, filepath, indent=4, sort_keys=True)
