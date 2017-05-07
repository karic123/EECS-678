import locale
locale.setlocale(locale.LC_ALL, 'en_US.UTF-8')

def FORMAT( val ):
    return locale.format("%d", val, grouping=True )

# Address Translation

#   8 bits      8 bits      8 bits
# ----------------------------------
# | 1st level | 2nd level | offset |
# ----------------------------------

# Virtual address format: 24 bits

#   4 bits    3 bits  1 bit
# ------------------------
# | frame # | unused | V |
# ------------------------ 

# Page table entry: 8 bits

def get_size_of_single_page( offset_bits ):
    return 2 ** offset_bits # 2^offset_bits


def get_virtual_address_format_bits( level_1_bits, level_2_bits, offset_bits ):
    return level_1_bits + level_2_bits + offset_bits


def get_size_of_virtual_address_space_bytes( virtual_address_format_bits ):
    return 2 ** virtual_address_format_bits # 2^virtual_address_format_bits


def get_size_of_physical_address_space_bytes( frame_bits, page_table_entry_bits ):
    return ( 2 ** frame_bits ) * ( 2 ** page_table_entry_bits ) # 2^frame_bits + 2^page_table_entry_bits


def get_bytes_to_MB( byteVal ):
    return ( byteVal ) / 1024 / 1024


def get_bytes_to_KB( byteVal ):
    return ( byteVal ) / 1024

# ----------------------------------------------------

first_level_bits        = 8
second_level_bits       = 8
offset_bits             = 8

page_table_entry_bits   = 8
frame_bits              = 4

virtual_address_format_bits = get_virtual_address_format_bits( first_level_bits, second_level_bits, offset_bits )
virtual_address_space_bytes = get_size_of_virtual_address_space_bytes( virtual_address_format_bits )

single_page_bytes = get_size_of_single_page( offset_bits )

size_of_physical_address_space_bytes = get_size_of_physical_address_space_bytes( frame_bits, page_table_entry_bits )


print( "1st level page size (bits):     " + str( first_level_bits ) )
print( "2nd level page size (bits):     " + str( second_level_bits ) )
print( "Size of offset (bits):          " + str( offset_bits ) )

print( "" )
print( "Page table entry size (bits):   " + str( page_table_entry_bits ) )
print( "Frame size (bits):              " + str( frame_bits ) )

print( "" )
print( "Size of a single page (bytes):  " + str( single_page_bytes ) )
print( "Virtual address format (bits):  " + str( virtual_address_format_bits ) )
print( "Virtual address space (bytes):  " + FORMAT( virtual_address_space_bytes ) + " (" + str( get_bytes_to_MB( virtual_address_space_bytes ) ) + " MB)" )
print( "Physical address space (bytes): " + FORMAT( size_of_physical_address_space_bytes ) + " (" + str( get_bytes_to_KB( size_of_physical_address_space_bytes ) ) + " KB)" )
