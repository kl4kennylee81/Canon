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
tree = ET.parse('build-win10/HelloWorld/HelloWorld.vcxproj')
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
# hack
hpp_files.append('resource.h')
for cpp_file in cpp_files:
	att = {}
	att['Include'] = "..\..\source\\" + cpp_file
	ET.SubElement(cpp_item_group, 'ClCompile', attrib=att)

# print hpp_files

for hpp_file in hpp_files:
	att = {}
	att['Include'] = "..\..\source\\" + hpp_file
	ET.SubElement(hpp_item_group, 'ClInclude', attrib=att)

indent(root)
# write to file
tree.write('build-win10/HelloWorld/HelloWorld.vcxproj', encoding="utf-8", xml_declaration=True)

# vcxproj.filters
# use white_list for vcxproj.filters
filtertree = ET.parse('build-win10/HelloWorld/HelloWorld.vcxproj.filters')
filterroot = filtertree.getroot()

# print white_listed

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

# print child_parent_map

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

# print flat_child_parent_map

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
			# print v
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




