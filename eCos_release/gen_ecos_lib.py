#!/usr/bin/python

import os
import sys
import re

###############################################################################
# config

PATH_MEMMAPFILE = 'config_option'

CONFIG_LIST_MEMMAP = [
		('User1', 	    '(Start , End)'),
		('User2', 	    '(Start , Size)'),
#		('Base1', 	    'ECOS :  Range = [  0MB-349MB], Size = 349MB'),
#		('Base2', 	    'ECOS :  Range = [  0MB-348MB], Size = 348MB'),
#		('Base3', 	    'ECOS :  Range = [  4MB-352MB], Size = 348MB'),
#		('Base4', 	    'ECOS :  Range = [164MB-512MB], Size = 348MB'),
#		('LGEAndroid', 	'ECOS :  Range = [  4MB-288MB], Size = 284MB'),
#		('MemReduce',   'ECOS :  Range = [  4MB- 62MB], Size =  58MB'),

		]

FILE_LIST_MEMEAP = {
		'MLT_H' : 'src/ecos/packages/hal/arm/arm9/galois/current/include/pkgconf/mlt_arm_arm9_galois_ram.h',
		'MLT_LDI' : 'src/ecos/packages/hal/arm/arm9/galois/current/include/pkgconf/mlt_arm_arm9_galois_ram.ldi',
		'VECTOR' : 'src/ecos/packages/hal/arm/arch/current/src/vectors.S.Berlin',
}

PIRNT_PATTERN = '   %2d  | %12s | %s '

UNIT_MEMORYSIZE = 1024 * 1024
CONFIG_OPTION_PATH = ''

MEM_SIZE_ECOS = 0
MEM_START_ECOS = 0
MEM_SIZE_DSP = 4

###############################################################################
# debug
_DEBUG = 0

###############################################################################
# function

def handle_config_menu():
	print "\n Note : CPU1 Memory = DSP Memory + ECOS Memory"
	print "        DSP Memory  = %s MB" % (str(MEM_SIZE_DSP))
	print "\n The list of ECOS Memory Map Input Option :"
	print "".join(["-" for i in range(80)])
	print " Index |  Option Tag  | ECOS Memory Map Input Parameter "
	print "".join(["-" for i in range(80)])
	print "\n".join([PIRNT_PATTERN % (CONFIG_LIST_MEMMAP.index(option) , option[0] , option[1]) for option in CONFIG_LIST_MEMMAP])
	print "".join(["-" for i in range(80)])
	input_num = -1
	while True:
		try:
			input_num = int(raw_input('Please select the index (%d - %d) : ' % (0, len(CONFIG_LIST_MEMMAP) - 1)))
		except ValueError:
			print "[ERROR] Input error"
			continue
		
		if 0 <= input_num < len(CONFIG_LIST_MEMMAP):
			break;
		else:
			print "[ERROR] Input out of index range"

	print "[OK] Selected %s = %s " % (CONFIG_LIST_MEMMAP[input_num][0], CONFIG_LIST_MEMMAP[input_num][1])
	
	global CONFIG_OPTION_PATH 
	CONFIG_OPTION_PATH = PATH_MEMMAPFILE + os.sep + CONFIG_LIST_MEMMAP[input_num][0] + os.sep
	
	return input_num

def copy_ecos_config_file(index, file_key):
	source_path = PATH_MEMMAPFILE + os.sep + CONFIG_LIST_MEMMAP[index][0] + os.sep +os.path.basename(FILE_LIST_MEMEAP[file_key])
	target_path = FILE_LIST_MEMEAP[file_key]
	cmd = 'cp -f %s %s' % (source_path, target_path)
	if _DEBUG: print cmd
	os.system(cmd)

def rename_file(source, newname):
	cmd = 'mv -f %s %s' % (source, os.path.dirname(source) + os.sep + newname)
	if _DEBUG: print cmd
	os.system(cmd)

def get_origin(dir_path):
	print "Update directory or file [%s] from Origin ..." % (dir_path)
	
	cmd = 'cp -rf %s %s' % (PATH_MEMMAPFILE + os.sep + 'Origin/*', dir_path)
	if _DEBUG: print cmd
	os.system(cmd)
	
def update_dir(dir_path):
	print "Update directory or file [%s] ..." % (dir_path)

	cmd = 'rm -rf %s' % (dir_path)
	if _DEBUG: print cmd
	os.system(cmd)
	
	cmd = 'svn up %s' % (dir_path)
	if _DEBUG: print cmd
	os.system(cmd)

def handle_modify_config_file1(filename, memory_start, memory_length):

	# modify mlt_arm_arm9_galois_ram.h
	print "generate %s ..." % (os.path.basename(filename))
	f_source = open(filename, 'r')
	f_content = f_source.read()
	f_source.close()
	
	f_content = re.sub(r'CYGMEM_REGION_ram \(0x________\)', 'CYGMEM_REGION_ram (0x%08X)' %(memory_start), f_content, 0)
	f_content = re.sub(r'CYGMEM_REGION_ram_SIZE    \(0x________\)', 'CYGMEM_REGION_ram_SIZE    (0x%08X)' %(memory_length), f_content, 0)
	f_content = re.sub(r'CYGMEM_SECTION_heap1_SIZE \(0x________', 'CYGMEM_SECTION_heap1_SIZE (0x%08X' %(memory_length), f_content, 0)

	f_target = open(filename, 'w')
	f_target.write(f_content)
	f_target.close()

def handle_modify_config_file2(filename, memory_start, memory_length):
	
	# modify mlt_arm_arm9_galois_ram.ldi
	print "generate %s ..." % (os.path.basename(filename))
	f_source = open(filename, 'r')
	f_content = f_source.read()
	f_source.close()
	
	f_content = re.sub(r'ORIGIN = 0x________, LENGTH = 0x________', 'ORIGIN = 0x%08X, LENGTH = 0x%08X' %(memory_start, memory_length), f_content, 0)
	f_content = re.sub(r'SECTION_fixed_vectors \(ram, 0x________, LMA_EQ_VMA\)', 'SECTION_fixed_vectors (ram, 0x%08X, LMA_EQ_VMA)' %(memory_start + 0x00000020), f_content, 0)
	f_content = re.sub(r'SECTION_rom_vectors \(ram, 0x________, LMA_EQ_VMA\)', 'SECTION_rom_vectors (ram, 0x%08X, LMA_EQ_VMA)' %(memory_start + 0x00040000), f_content, 0)

	f_target = open(filename, 'w')
	f_target.write(f_content)
	f_target.close()
	
def handle_ecos_config_user(index):
    
	if index == 0:
        # user1 
        # get input 
        # 1. memory start
        # 2. memory end
		try:
			memory_start = int(raw_input('Please input ECOS memory start (MB, after DSP Memory) : ')) * UNIT_MEMORYSIZE
			memory_end = int(raw_input('Please input ECOS memory end (MB) : ')) * UNIT_MEMORYSIZE
		except ValueError:
			print "[ERROR] Input error"
			sys.exit(1)
			
		memory_length = memory_end - memory_start
		if memory_length <= 0:
			print "[ERROR] Input error, length <= 0"
			sys.exit(1)
			
	elif index == 1:
        # user2 
        # get input 
        # 1. memory start
        # 2. memory length
		try:
			memory_start = int(raw_input('Please input ECOS memory start (MB, after DSP Memory) : ')) * UNIT_MEMORYSIZE
			memory_length = int(raw_input('Please input ECOS memory length (MB, exclude DSP Memory) : ')) * UNIT_MEMORYSIZE
		except ValueError:
			print "[ERROR] Input error"
			sys.exit(1)
		
		memory_end = memory_start + memory_length

	else:
		return

	print
	print 'memory start  = 0x%08X' % (memory_start)
	print 'memory end    = 0x%08X' % (memory_end)
	print 'memory length = 0x%08X' % (memory_length)
	print
	
	filename = CONFIG_OPTION_PATH + os.path.basename(FILE_LIST_MEMEAP['MLT_H'])
	handle_modify_config_file1(filename, memory_start, memory_length)
	filename = CONFIG_OPTION_PATH + os.path.basename(FILE_LIST_MEMEAP['MLT_LDI'])
	handle_modify_config_file2(filename, memory_start, memory_length)
	
	global MEM_START_ECOS
	MEM_START_ECOS = memory_start / UNIT_MEMORYSIZE
	global MEM_SIZE_ECOS
	MEM_SIZE_ECOS = memory_length / UNIT_MEMORYSIZE

def handle_ecos_linkfile(filename_source, tag1, tag2):
	filename_target = os.path.dirname(filename_source) + os.sep + os.path.basename(filename_source).split('.')[0] + tag1 + tag2 + '.' + filename_source.split('.')[1]
	print 'handle ecos linkfile : %s => %s' % (filename_source, os.path.basename(filename_target))
	try:
		f_source = open(filename_source, 'r')
		f_target = open(filename_target, 'w')
		lines = f_source.read()
		lines = re.sub(r'STARTUP\(vectors\.o\)', r'STARTUP(vectors%s.o)' % tag1, lines, 0)
		lines = re.sub(r'INPUT\(extras\.o\)', r'INPUT(extras%s.o)' % tag2, lines, 0)
		lines = re.sub(r'GROUP\(libtarget\.a', r'GROUP(impure.o libtarget%s.a' % tag2, lines, 0)
		f_target.write(lines)
	finally:
		f_source.close()
		f_target.close()
		os.remove(filename_source)	
	return

def build_ecos_lib():
	option_index = handle_config_menu()
	if _DEBUG: print option_index

	#update_dir(CONFIG_OPTION_PATH)
	get_origin(CONFIG_OPTION_PATH)
	if not os.path.isdir(CONFIG_OPTION_PATH):
		print "[ERROR] Option [%s] is not defined" % (CONFIG_LIST_MEMMAP[option_index][0])
		sys.exit(1)

	handle_ecos_config_user(option_index)
	
	'''
	I found that os.getenv always get the external env value
	os.putenv only change env value of internal python
	'''
	root_path = os.getenv('PWD')
	newpath = ':%s/toolset/arm-marvell-eabi/bin' % root_path
	newpath += ':%s/toolset/tools/bin' % root_path
	os.putenv('PATH', os.getenv('PATH') + ':%s' % newpath)
	if _DEBUG: print os.getenv('PATH')

	os.putenv('ECOS_REPOSITORY', '%s/src/ecos/packages' % root_path)
	if _DEBUG: os.system('export')
       
	copy_ecos_config_file(option_index, 'MLT_H')
	copy_ecos_config_file(option_index, 'MLT_LDI')
	
	update_dir(FILE_LIST_MEMEAP['VECTOR'])
	rename_file(FILE_LIST_MEMEAP['VECTOR'], 'vectors.S')

	os.system('rm -rf release')
	os.system('mkdir -p release')

	print "\nstart building eCos lib for %s --->\n" % CONFIG_LIST_MEMMAP[option_index][0]

	######################################################
	# build default_Galois

	os.system('rm -rf build')
	os.system('mkdir -p build')
	if _DEBUG: print os.path.abspath('.')
	os.chdir('%s/%s' %(root_path, 'build'))
	if _DEBUG: print os.path.abspath('.')

	os.system('ecosconfig new galois default_Galois')
	os.system('ecosconfig tree')
	os.system('make -s')

	if _DEBUG: print os.path.abspath('.')
	os.chdir('%s' %(root_path))
	if _DEBUG: print os.path.abspath('.')

	source_path = root_path + '/build/install/*'
	target_path = root_path + '/release/'
	cmd = 'cp -rf %s %s' % (source_path, target_path)
	if _DEBUG: print cmd
	os.system(cmd)

	rename_file(root_path + '/release/lib/extras.o', 'extras_CPU1.o')
	rename_file(root_path + '/release/lib/libextras.a', 'libextras_CPU1.a')
	rename_file(root_path + '/release/lib/libtarget.a', 'libtarget_CPU1.a')
	rename_file(root_path + '/release/lib/vectors.o', 'vectors_Berlin.o')
	
	handle_ecos_linkfile(root_path + '/release/lib/target.ld', '_Berlin', '_CPU1')

	######################################################
	# build default_for_sysmon

	if _DEBUG: print os.path.abspath('.')
	os.chdir('%s/%s' %(root_path, 'build'))
	if _DEBUG: print os.path.abspath('.')

	os.system('make clean -s')
	os.system('ecosconfig new galois default_for_sysmon')
	os.system('ecosconfig tree')
	os.system('make -s')

	if _DEBUG: print os.path.abspath('.')
	os.chdir('%s' %(root_path))
	if _DEBUG: print os.path.abspath('.')

	source_path = root_path + '/build/install/lib/*'
	target_path = root_path + '/release/lib/'
	cmd = 'cp -rf %s %s' % (source_path, target_path)
	if _DEBUG: print cmd
	os.system(cmd)

	rename_file(root_path + '/release/lib/extras.o', 'extras_CPU1_SysMonitor.o')
	rename_file(root_path + '/release/lib/libextras.a', 'libextras_CPU1_SysMonitor.a')
	rename_file(root_path + '/release/lib/libtarget.a', 'libtarget_CPU1_SysMonitor.a')

	handle_ecos_linkfile(root_path + '/release/lib/target.ld', '_Berlin', '_CPU1_SysMonitor')

	######################################################
	# remove some files and generate packet
	os.remove(root_path + '/release/include/pkgconf/hal_arm_arm9_galois.h')
	os.remove(root_path + '/release/lib/vectors.o')
#	os.system('cp -f %s %s' % (root_path + '/release/include/pkgconf/mlt_arm_arm9_galois_ram.h', root_path + '/release/include/pkgconf/mlt_arm_arm9_galois_ram.h.CPU1'))
#	os.system('cp -f %s %s' % (root_path + '/release/include/pkgconf/mlt_arm_arm9_galois_ram.ldi', root_path + '/release/include/pkgconf/mlt_arm_arm9_galois_ram.ldi.CPU1'))

	print
	print "MEM_START DSP   = %d (MB)" % (MEM_START_ECOS - MEM_SIZE_DSP)
	print "MEM_SIZE  DSP   = %d (MB)" % (MEM_SIZE_DSP)
	print "MEM_START ECOS  = %d (MB)" % (MEM_START_ECOS)
	print "MEM_SIZE  ECOS  = %d (MB)" % (MEM_SIZE_ECOS)
	print "MEM_START CPU1  = %d (MB)" % (MEM_START_ECOS - MEM_SIZE_DSP)
	print "MEM_SIZE  CPU1  = %d (MB)" % (MEM_SIZE_ECOS + MEM_SIZE_DSP)

	release_packet_name = 'eCos_%d_%d.tar.gz' % (MEM_START_ECOS, MEM_START_ECOS + MEM_SIZE_ECOS)
	os.chdir('%s/release' %(root_path))
	os.system('tar -czf ../%s --remove-files *' % release_packet_name)
	os.chdir('%s' %(root_path))
	print "\n[OK] The eCos lib is generated."
	print release_packet_name
	print 

if __name__ == "__main__":
	build_ecos_lib()

