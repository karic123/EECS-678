import locale, numpy, math
locale.setlocale(locale.LC_ALL, 'en_US.UTF-8')

# ----------------------------------------------------

def FORMAT( val ):
    return locale.format("%d", val, grouping=True )

    
def get_bytes_to_KB( byteVal ):
    return ( byteVal ) / 1024
    
def get_bytes_to_MB( byteVal ):
    return ( byteVal ) / 1024 / 1024
    
def get_bytes_to_GB( byteVal ):
    return ( byteVal ) / 1024 / 1024 / 1024

def get_bytes_to_TB( byteVal ):
    return ( byteVal ) / 1024 / 1024 / 1024 / 1024

def get_KB_to_bytes( byteVal ):
    return ( byteVal ) * 1024
    
# ----------------------------------------------------
# Disk accesses required

# This is just an assumption based on the question given and the solution.
def get_accesses_required_linked_allocation( access_bytes, block_size_KB ):
    disk_block_bytes = get_KB_to_bytes( block_size_KB )
    return math.ceil( access_bytes / disk_block_bytes )

# ----------------------------------------------------
access_bytes = 20680.0
disk_block_KB = 4.0

accesses_required = get_accesses_required_linked_allocation( access_bytes, disk_block_KB )

print( "Access how many bytes:      " + FORMAT( access_bytes ) )
print( "Disk block size (KB):       " + str( disk_block_KB ) )
print( "Accesses required:          " + str( accesses_required ) )
print( "" )

# ----------------------------------------------------
# Maximum disk size of filesystem, and maximum file size of a file

def get_maximum_disk_size_bytes( block_size_bytes, block_pointer_size_bits ):
    return (2 ** block_pointer_size_bits ) * block_size_bytes

def get_maximum_file_size_bytes( block_size_bytes, direct_pointer_count, indirect_pointer_count, doubly_indirect_pointer_count ):
    direct_pointer_bytes            = ( direct_pointer_count * block_size_bytes )
    indirect_pointer_bytes          = ( indirect_pointer_count * block_size_bytes ) * ( block_size_bytes / 4 )
    doubly_indirect_pointer_count   = ( doubly_indirect_pointer_count * block_size_bytes ) * ( ( block_size_bytes / 4 ) ** 2 )
    return direct_pointer_bytes + indirect_pointer_bytes + doubly_indirect_pointer_count

# ----------------------------------------------------

block_size_bytes = 2048
block_pointer_size_bits = 32
direct_pointer_count = 10
indirect_pointer_count = 1
doubly_indirect_pointer_count = 1

maximum_disk_size_bytes = get_maximum_disk_size_bytes( block_size_bytes, block_pointer_size_bits )

maximum_file_size_bytes = get_maximum_file_size_bytes( block_size_bytes, direct_pointer_count, indirect_pointer_count, doubly_indirect_pointer_count )


print( "Block size (bytes):         " + FORMAT( block_size_bytes ) )
print( "Block pointer size (bits):  " + str( block_pointer_size_bits ) )
print( "Maximum disk size (bytes):  " + FORMAT( maximum_disk_size_bytes ) + " (" + str( get_bytes_to_TB( maximum_disk_size_bytes ) ) + " TB)" )
print( "" )
print( "Block size (bytes):         " + FORMAT( block_size_bytes ) )
print( "Direct pointers:            " + str( block_pointer_size_bits ) )
print( "Indirect pointers:          " + str( indirect_pointer_count ) )
print( "Doubly indirect pointers:   " + str( doubly_indirect_pointer_count ) )
print( "Maximum file size (bytes):  " + FORMAT( maximum_file_size_bytes ) + " (" + str( get_bytes_to_MB( maximum_file_size_bytes ) ) + " MB)"  )





