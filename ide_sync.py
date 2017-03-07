# make dict representation of the pbxproj XCode config file:

xcode_structure = {}
GROUP_START = '/* Begin PBXGroup section */'
GROUP_END = '/* End PBXGroup section */'
COMMENT_START = '/*'
COMMEND_END = '*/'
LIST_END = ');'
should_parse = False
grab_children = False
child_buffer = []

with open('build-apple/Canon.xcodeproj/project.pbxproj') as xcode_config:
	for line in xcode_config:

		if GROUP_START in line:
			should_parse = True
			continue
			# start parsing LOL
		elif GROUP_END in line:
			break # we are done

		if should_parse:
			if grab_children:
				if LIST_END in line:
					grab_children = False
					continue

				ind1 = line.index(COMMENT_START)
				ind2 = line.index(COMMEND_END)
				child = line[ind1+3:ind2-1]
				child_buffer.append(child)

			elif 'children' in line:
				grab_children = True
				child_buffer = [] # some don't have names, so we also reset here
			elif 'name' in line:
				actual_name = line[line.index('name')+7:len(line)-2]
				xcode_structure[actual_name] = child_buffer
				child_buffer = []
				# print actual_name, xcode_structure[actual_name]

# clean up - we remove all branches that do not eventually lead to a cpp or hpp file
cpp_files = []
hpp_files = []
white_listed = set() # stores good keys
# print xcode_structure
for k in xcode_structure.keys():

	for kid in xcode_structure[k]:

		if '.cpp' in kid or '.hpp' or '.h' in kid:
			if '.cpp' in kid:
				cpp_files.append(kid)
				white_listed.add(k)
			elif '.hpp' in kid or '.h' in kid:
				hpp_files.append(kid)
				white_listed.add(k)

# retrieve parents of white_listed
old_white_listed = set()
while (len(white_listed) != len(old_white_listed)):
	# print white_listed
	old_white_listed = set(white_listed) # copy
	for k in xcode_structure.keys():
		if k not in white_listed:
			for kid in xcode_structure[k]:
				if kid in white_listed:
					white_listed.add(k)

# print xcode_structure.keys()
# print white_listed

# now, we sync the .vcxproj of the visual studio setup.
import xml.etree.ElementTree as ET
ET.register_namespace('', 'http://schemas.microsoft.com/developer/msbuild/2003')
tree = ET.parse('build-win10/HelloWorld/Helloworld.vcxproj')
root = tree.getroot()
# print root.tag
# item_group = root.findall('Project')

cpp_item_group = None
hpp_item_group = None
found_cpp = False
found_hpp = False
kid_dna = None
for child in root:
	if 'ItemGroup' in child.tag:
		for kid in child:
			if found_cpp:
				cpp_item_group = child
				found_cpp = False
				break
			elif found_hpp:
				hpp_item_group = child
				found_hpp = False
				break

			for k, v in kid.items():
				if '.cpp' in v:
					found_cpp = True
					kid_dna = kid
					break
					# print v
				elif '.hpp' in v or '.h' in v:
					found_hpp = True
					kid_dna = kid
					break
					# print v

# print cpp_item_group
# print hpp_item_group
# print kid_dna

# generate clcompiles
cpp_item_group.clear()
hpp_item_group.clear()

# print kid_dna

# use cpp_files and hpp_files for vcxproj
for cpp_file in cpp_files:
	att = {}
	att['Include'] = "..\..\source\\" + cpp_file
	ET.SubElement(cpp_item_group, 'ClCompile', attrib=att)

for hpp_file in hpp_files:
	att = {}
	att['Include'] = "..\..\source\\" + hpp_file
	ET.SubElement(hpp_item_group, 'ClInclude', attrib=att)

# write to file
tree.write('build-win10/HelloWorld/Test.vcxproj', encoding="utf-8", xml_declaration=True)

# vcxproj.filters
# use white_liste for vcxproj.filters
filtertree = ET.parse('build-win10/HelloWorld/Helloworld.vcxproj.filters')
filterroot = filtertree.getroot()






