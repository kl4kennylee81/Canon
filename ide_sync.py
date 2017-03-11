# from http://effbot.org/zone/element-lib.htm#prettyprint
def indent(elem, level=0):
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

## PART ONE: COLLECT DATA FROM XCODE
xcode_structure = {}
should_parse = False
grab_children = False
child_buffer = []

with open('build-apple/Canon.xcodeproj/project.pbxproj') as xcode_config:
	for line in xcode_config:

		# the part of the file that we're interested in
		if '/* Begin PBXGroup section */' in line:
			should_parse = True
			continue
		elif '/* End PBXGroup section */' in line:
			break

		if should_parse:
			if grab_children:
				if ');' in line: # denotes end of children listings
					grab_children = False
					continue

				ind1 = line.index('/*')
				ind2 = line.index('*/')
				child = line[ind1+3:ind2-1] # specific af, dwai
				child_buffer.append(child)

			elif 'children' in line:
				grab_children = True
				child_buffer = [] # some don't have 'name', so we also reset here
			
			elif 'name' in line:
				actual_name = line[line.index('name')+7:len(line)-2] # slicing to get rid of comments
				xcode_structure[actual_name] = child_buffer
				child_buffer = []


# clean up - we remove all branches that do not eventually lead to a cpp or hpp file
cpp_files = []
hpp_files = []
white_listed = set() # stores good keys

for k in xcode_structure.keys():

	for kid in xcode_structure[k]:

		if '.cpp' in kid or '.hpp' or '.h' in kid:
			if '.cpp' in kid:
				cpp_files.append(kid)
				white_listed.add(k)
			elif '.hpp' in kid or '.h' in kid:
				hpp_files.append(kid)
				white_listed.add(k)


# retrieve parents of white_listed until no changes in set occurs
old_white_listed = set()
while (len(white_listed) != len(old_white_listed)):

	old_white_listed = set(white_listed) # copy
	for k in xcode_structure.keys():
		if k not in white_listed:
			for kid in xcode_structure[k]:
				if kid in white_listed:
					white_listed.add(k)


## PART TWO: SYNC VCXPROJ

# now, we sync the .vcxproj of the visual studio setup.
import xml.etree.ElementTree as ET
ET.register_namespace('', 'http://schemas.microsoft.com/developer/msbuild/2003')
tree = ET.parse('build-win10/HelloWorld/HelloWorld.vcxproj')
root = tree.getroot()

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
				elif '.hpp' in v or '.h' in v:
					found_hpp = True
					kid_dna = kid
					break

# generate clcompiles
cpp_item_group.clear()
hpp_item_group.clear()

# use cpp_files and hpp_files for vcxproj
# hack to automatically include resource.h
ET.SubElement(hpp_item_group, 'ClInclude', attrib={'Include': 'resource.h'})

for cpp_file in cpp_files:
	att = {'Include': "..\..\source\\" + cpp_file}
	ET.SubElement(cpp_item_group, 'ClCompile', attrib=att)

for hpp_file in hpp_files:
	att = {'Include': "..\..\source\\" + hpp_file}
	ET.SubElement(hpp_item_group, 'ClInclude', attrib=att)

indent(root)
# write to file
tree.write('build-win10/HelloWorld/HelloWorld.vcxproj', encoding="utf-8", xml_declaration=True)


## STEP THREE: SYNC FILTERS

filtertree = ET.parse('build-win10/HelloWorld/HelloWorld.vcxproj.filters')
filterroot = filtertree.getroot()

filter_parent = None
include_parent = None
compile_parent = None

for child in filterroot:
	if 'ItemGroup' in child.tag:
		for kid in child:
			if 'Filter' in kid.tag:
				filter_parent = child
				continue
			elif 'ClInclude' in kid.tag:
				include_parent = child
			elif 'ClCompile' in kid.tag:
				compile_parent = child


# create child-parent mapping
child_parent_map = {}
mapstack = ['Source']
while len(mapstack) > 0:
	popped = mapstack.pop()
	klist = xcode_structure[popped]
	for k in klist:
		child_parent_map[k] = popped
		if not ('.cpp' in k or '.hpp' in k or '.h' in k):
			mapstack.append(k)


# create flattened child-path mapping
flat_child_parent_map = {}
for child in child_parent_map.keys():
	if not ('.cpp' in child or '.hpp' in child or '.h' in child):
		continue
	value = child_parent_map[child]
	path = value
	while value in child_parent_map:
		path = child_parent_map[value] + '\\' + value
		value = child_parent_map[value]
	flat_child_parent_map[child] = path


# pin source folder to top
filter_parent.clear()
compile_parent.clear()
att = {}
parent_folder = 'Source'

# generate other filter folders at top
unique_paths = set()
for k,v in flat_child_parent_map.items():
	unique_paths.add(v)

ET.SubElement(filter_parent, 'Filter', attrib={'Include': 'Resource Files'}).text=' '
for path in unique_paths:
	ET.SubElement(filter_parent, 'Filter', attrib={'Include': path}).text=' '

# preserve resource.h
remove_list = []
for child in include_parent:
	for k,v in child.items():
		if 'resource.h' not in v:
			remove_list.append(child)
		else:
			for f in child:
				f.text = parent_folder # put it into source


# clear the header files
for c in remove_list:
	include_parent.remove(c)

# generate cl includes
for c in flat_child_parent_map:
	if '.cpp' in c:
		path = "..\..\source\\" + c
		k = ET.SubElement(include_parent, 'ClCompile', attrib={'Include': path})
		k.text=' '
		ET.SubElement(k, 'Filter').text=flat_child_parent_map[c]
	elif '.hpp' in c or '.h' in c:
		path = "..\..\source\\" + c
		k = ET.SubElement(compile_parent, 'ClInclude', attrib={'Include': path})
		k.text=' '
		ET.SubElement(k, 'Filter').text=flat_child_parent_map[c]

indent(filterroot)
# write to file
filtertree.write('build-win10/HelloWorld/HelloWorld.vcxproj.filters', encoding="utf-8", xml_declaration=True)