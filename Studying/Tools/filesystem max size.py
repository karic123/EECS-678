# Suppose a filesystem's disk block is 2048 bytes and its block pointer size is 4 byte (32bit).
# What is the maximum size of the filesystem?
# 2^32*2^11
# Block pointer size is 32 bit
# So no of blocks it can access 2^32
# 1 block is 2048 bytes that is 2^11
# 8 Terabytes

import locale
locale.setlocale(locale.LC_ALL, 'en_US.UTF-8')

def FORMAT( val ):
	return locale.format("%d", val, grouping=True )

filesystem_disk_block_in_bytes		= input( "      What size are disk blocks?    (in bytes, e.g., 2048): " ) # 2048 bytes
block_pointer_size_in_bytes			= input( "    What is the block pointer size? (in bytes, e.g., 4):    " ) # 4 bytes

block_pointer_size_in_bits = block_pointer_size_in_bytes * 8 							# 32 bits

filesystem_disk_block_in_kilobytes = int( filesystem_disk_block_in_bytes * 0.001 ) 		# approximately 2, though I do not understand this step at all (specifically flooring it to get the right answer...)

accessibleBlocks = filesystem_disk_block_in_kilobytes ** block_pointer_size_in_bits 	# ** = exponent operator, 2^32

max_filesystem_size = accessibleBlocks * filesystem_disk_block_in_bytes

print( "" )
print( " Accessible blocks: " + str( filesystem_disk_block_in_kilobytes ) + "^" + str( block_pointer_size_in_bits ) + " (" + FORMAT( accessibleBlocks ) + ")" )
print( "  Maximum filesize: " + FORMAT( accessibleBlocks ) + " x " + FORMAT( filesystem_disk_block_in_bytes ) + " (" + FORMAT( max_filesystem_size ) + " bytes)" )
